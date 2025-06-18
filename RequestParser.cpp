#include "RequestParser.hpp"

RequestParser::RequestParser() : state(START_LINE), contentLength(0), chunkSize(0), _location(NULL), _serverConfig(NULL), maxBodySize(0){}

void RequestParser::reset() {
    state = START_LINE;
    request = HttpRequest();
    buffer.clear();
    contentLength = 0;
	_location = NULL;
	_serverConfig = NULL;
	maxBodySize = 0;
}

bool RequestParser::isDone() const {
    return state == DONE;
}

const HttpRequest RequestParser::getRequest() const {
    return request;
}

void RequestParser::setLocation (const Location *location){
	_location = location;
}
void RequestParser::setServerConfig(const ServerConfig *serverConfig){
	_serverConfig = serverConfig;
}
const ServerConfig *RequestParser::getServerConfig(){
	return _serverConfig;
}

RequestParser::ParseResult RequestParser::parse(const char* data, size_t len) throw (SourceAndRequestException){

    buffer.append(data, len);

    while (true) {
        switch (state) {
            case START_LINE:
                if (!parseStartLine()) return INCOMPLETE;
				_parseUrl();
                state = HEADERS;
				return GET_CONFIGS;

            case HEADERS:
                if (!parseHeaders()) return INCOMPLETE;
                state = BODY;
                break;

            case BODY:
                if (!parseBody()) return INCOMPLETE;
                state = DONE;
                return COMPLETE;

            case DONE:
                return COMPLETE;

            case ERROR:
                return BAD;
        }
    }
}

bool RequestParser::parseStartLine() {
    size_t pos = buffer.find("\r\n");
    if (pos == std::string::npos) return false;

    std::istringstream line(buffer.substr(0, pos));
    if (!(line >> request.method >> request.uri >> request.http_version)){
		state = DONE;
		throw (SourceAndRequestException("Error reading request start line", 400));
	}

    buffer.erase(0, pos + 2);
    return true;
}

bool RequestParser::parseHeaders() {
    size_t pos;
    while ((pos = buffer.find("\r\n")) != std::string::npos) {
        std::string line = buffer.substr(0, pos);
        buffer.erase(0, pos + 2);
        if (line.empty()) return true; // End of headers

        size_t colon = line.find(":");
        if (colon == std::string::npos){
			state = DONE;
			throw (SourceAndRequestException("Error parsing headers", 400));
		}
        std::string key = line.substr(0, colon);
        std::string value = line.substr(colon + 2);
        request.headers[key] = value;
    }
    return false;
}

bool RequestParser::parseBody() {
    if (request.method != "POST") return true;

	if (maxBodySize == 0)
		getMaxBodySize();
	std::map<std::string, std::string>::iterator it = request.headers.find("Transfer-Encoding");
	if (it != request.headers.end() && it->second == "chunked") {
		if (!_handleChunkedInput())
			return false;
		contentLength = request.body.size();
	}
    else {
		_checkContentLength(request.headers.find("Content-Length"));
		if (buffer.size() < contentLength) return false;
		request.body = buffer.substr(0, contentLength);
	}

    buffer.erase(0, contentLength);
    return true;
}

void RequestParser::_checkContentLength(std::map<std::string, std::string>::iterator it){
	if (it == request.headers.end()){
		state = DONE;
		throw (SourceAndRequestException("No Content-length header for not chunked request", 400));
	}
	for (size_t i = 0; i < it->second.size(); ++i) {
		if (!std::isdigit(it->second[i])){
			state = DONE;
			throw (SourceAndRequestException("Error in the Content-length header", 400));
		}
	}
	contentLength = atoi(it->second.c_str());
	if (contentLength > maxBodySize){
		state = DONE;
		throw (SourceAndRequestException("Body size exceeds allowed maximum", 403));
	}
}

bool RequestParser::_handleChunkedInput(){
	std::string line;
	size_t pos;
	if (chunkSize == 0){
		if ((pos = buffer.find("\r\n")) != std::string::npos){
			_parseChunkSize(buffer.substr(0, pos));
			buffer.erase(0, pos + 2);
			if (chunkSize == 0){
				state = DONE;
				if (buffer == "\r\n")
					return true;
				throw SourceAndRequestException("Malformed chunk", 400);
			}
		}
		else
			return false;
	}
	if ((pos = buffer.find("\r\n")) != std::string::npos){
		if (buffer.substr(0, pos).size() != chunkSize){
			state = DONE;
			throw SourceAndRequestException("Malformed chunk", 400);
		}
		request.body.append(buffer.substr(0, pos));
		buffer.erase(0, pos + 2);
	}
	return false;
}

void RequestParser::_parseChunkSize(const std::string& hexStr){
	if (hexStr.size() == 0){
		state = DONE;
		throw SourceAndRequestException("Malformed chunk", 400);
	}
	for (size_t i = 0; i < hexStr.size(); ++i) {
		if (!std::isxdigit(hexStr[i])){
			state = DONE;
			throw (SourceAndRequestException("Malformed chunk", 400));
		}
	}
    chunkSize = strtol(hexStr.c_str(), NULL, 16);
	if (request.body.size() + chunkSize > maxBodySize){
		state = DONE;
		throw SourceAndRequestException("Body size exceeds allowed maximum", 403);
	}
}

void RequestParser::_parseUrl() {
	size_t 		hostStart;
	size_t		hostEnd;
	size_t		pathEnd;
	std::string	url = request.uri;

	hostStart = url.find("://");
	if (hostStart != std::string::npos)
		hostStart += 3;
	else
		hostStart = 0;

	hostEnd = url.find_first_of("/:?#", hostStart);
	pathEnd = url.find_first_of("?#", hostEnd);
	if (hostEnd == std::string::npos)
		hostEnd = url.length();

	request.hostname = url.substr(hostStart, hostEnd - hostStart);
	request.path = url.substr(hostEnd, pathEnd - hostEnd);
	if (request.path == "")
		request.path = "/";
}

void RequestParser::getMaxBodySize(){
	maxBodySize = Config::getClientMaxBodySize(*_serverConfig, _location);
}
