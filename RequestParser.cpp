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

RequestParser::ParseResult RequestParser::parse(const char* data, size_t len) {
    if(data && len > 0)
		_buffer.append(data, len);
    while (true) {
        switch (_state) {
            case START_LINE:
                if (!parseStartLine()) return INCOMPLETE;
                _parseUrl();
                _state = HEADERS;
				return URL_READY;
                // break;
            case HEADERS:
                if (!parseHeaders()) return INCOMPLETE;
                _state = BODY;
                break;
            case BODY:
                if (!parseBody()) return INCOMPLETE;
                _state = DONE;
                return COMPLETE;
            case DONE:
                return COMPLETE;
            case ERROR:
                return BAD;
        }
    }
}
bool RequestParser::parseStartLine() {
    size_t pos = _buffer.find("\r\n");
    if (pos == std::string::npos) return false;
    std::istringstream line(_buffer.substr(0, pos));
    if (!(line >> _request.method >> _request.uri >> _request.http_version)) {
        _state = ERROR;
        return false;
    }
    _buffer.erase(0, pos + 2);
    return true;
}
bool RequestParser::parseHeaders() {
    size_t pos;
    while ((pos = _buffer.find("\r\n")) != std::string::npos) {
        std::string line = _buffer.substr(0, pos);
        _buffer.erase(0, pos + 2);
        if (line.empty()) return true; // End of headers
        size_t colon = line.find(":");
        if (colon == std::string::npos) {
            _state = ERROR;
            return false;
        }
        std::string key = line.substr(0, colon);
        std::string value = line.substr(colon + 2);
        _request.headers[key] = value;
    }
    return false; // wait for more data
}
bool RequestParser::parseBody() {
    if (_request.method != "POST") return true;
    std::map<std::string, std::string>::iterator it = _request.headers.find("Content-Length");
    if (it == _request.headers.end()) {
        throw SourceAndRequestException("No Content Length header", 400);
    }
    contentLength = atoi(it->second.c_str());
	if (contentLength > _maxBody)
		throw SourceAndRequestException("Body too big", 403);
    if (_buffer.size() < contentLength) return false;
    _request.body = _buffer.substr(0, contentLength);
    _buffer.erase(0, contentLength);
    return true;
}
void RequestParser::_parseUrl() {
    size_t      hostStart;
    size_t      hostEnd;
    size_t      pathEnd;
    std::string url = _request.uri;
    hostStart = url.find("://");
    if (hostStart != std::string::npos)
        hostStart += 3;
    else
        hostStart = 0;
    hostEnd = url.find_first_of("/:?#", hostStart);
    pathEnd = url.find_first_of("?#", hostEnd);
    if (hostEnd == std::string::npos)
        hostEnd = url.length();
    _request.hostname = url.substr(hostStart, hostEnd - hostStart);
    _request.path = url.substr(hostEnd, pathEnd - hostEnd);
    if (_request.path == "")
        _request.path = "/";
}

RequestParser::ParseResult RequestParser::continueParsing(){
	return parse(NULL, 0);
}








