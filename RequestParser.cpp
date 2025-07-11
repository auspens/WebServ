#include "RequestParser.hpp"
RequestParser::RequestParser() : _state(START_LINE), contentLength(0) {}
void RequestParser::reset() {
    _state = START_LINE;
    _request = HttpRequest();
    _buffer.clear();
    contentLength = 0;
}
bool RequestParser::isDone() const {
    return _state == DONE;
}
const HttpRequest RequestParser::getRequest() const {
    return _request;
}

void RequestParser::setMaxBody(size_t size){
	_maxBody = size;
}

RequestParser::ParseResult RequestParser::parse(const char* data, size_t len) throw(SourceAndRequestException) {
    if(data && len > 0)
		_buffer.append(data, len);
    while (true) {
        switch (_state) {
            case START_LINE:
                if (!parseStartLine(data, len))
					return _buffer.empty() && len == 0 ? EMPTY : INCOMPLETE;
                _parseUrl();
				std::cout << "Request path: " << _request.path << std::endl;
                _state = HEADERS;
				return URL_READY;
            case HEADERS:
                if (!parseHeaders(data, len)) return INCOMPLETE;
                _state = BODY;
                break;
            case BODY:
                if (!parseBody(data, len)) return INCOMPLETE;
                _state = DONE;
                return COMPLETE;
            case DONE:
                return COMPLETE;
			case ERROR:
				throw SourceAndRequestException("Bad request", 400); // Nicer if parser doesn't throw errors but saves error and returns state BAD
        }
    }
}

bool RequestParser::parseStartLine(const char *data, size_t len) throw(SourceAndRequestException) {
	size_t pos = _buffer.find("\r\n");

	if (pos == std::string::npos)
		return checkForError(data, len, false);

	std::istringstream line(_buffer.substr(0, pos));

	if (!(line >> _request.method >> _request.uri >> _request.http_version))
		throw SourceAndRequestException("Bad request", 400);
	if (_request.http_version != "HTTP/1.1")
		throw SourceAndRequestException("Incorrect http version", 503);
	if (_request.method != "POST" && _request.method != "GET" && _request.method != "DELETE")
		throw SourceAndRequestException("Incorrect http version", 400);

	_buffer.erase(0, pos + 2);
	return true;
}

bool RequestParser::parseHeaders(const char *data, size_t len) throw(SourceAndRequestException) {
    size_t pos;
    while ((pos = _buffer.find("\r\n")) != std::string::npos) {
        std::string line = _buffer.substr(0, pos);
        _buffer.erase(0, pos + 2);
        if (line.empty()) {
			if (_request.headers.find("Host") == _request.headers.end())
				throw SourceAndRequestException("No Host header", 400);
			return true; // End of headers
		}
        size_t colon = line.find(":");
        if (colon == std::string::npos)
            checkForError(data, len, true);
        std::string key = line.substr(0, colon);
        std::string value = line.substr(colon + 2);
        _request.headers[key] = value;
    }
    return checkForError(data, len, false); // wait for more data
}

bool RequestParser::parseBody(const char *data, size_t len) throw(SourceAndRequestException) {
    if (_request.method != "POST") return true;
    std::map<std::string, std::string>::iterator it = _request.headers.find("Content-Length");
    if (it == _request.headers.end()) {
        throw SourceAndRequestException("No Content Length header", 400);
    }
    contentLength = atoi(it->second.c_str());
	if (contentLength > _maxBody)
		throw SourceAndRequestException("Body too big", 413);
    if (_buffer.size() < contentLength) return checkForError(data, len, false);
    _request.body = _buffer.substr(0, contentLength);
    _buffer.erase(0, contentLength);
    return true;
}

void RequestParser::_parseUrl() {
	size_t		hostStart;
	size_t		hostEnd;
	size_t		pathEnd;
	std::string	url = _request.uri;

	hostStart = url.find("://");
	if (hostStart != std::string::npos)
		hostStart += 3;
	else
		hostStart = 0;

	hostEnd = url.find_first_of("/:?#", hostStart);
	if (hostEnd == std::string::npos)
		hostEnd = url.length();

	pathEnd = url.find_first_of("?#", hostEnd);
	_request.hostname = url.substr(hostStart, hostEnd - hostStart);
	_request.path = url.substr(hostEnd, pathEnd - hostEnd);

	if (_request.path == "")
		_request.path = "/";
}

RequestParser::ParseResult RequestParser::continueParsing() {
	return parse(NULL, 0);
}

bool RequestParser::checkForError(const char *data, size_t len, bool errorFound) {
	if (errorFound || (data && len == 0))
		_state = ERROR;
	return false;
}
