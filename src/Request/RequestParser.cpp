#include "RequestParser.hpp"
RequestParser::RequestParser(){
	_state = START_LINE;
    _request = HttpRequest();
    _buffer.clear();
    _contentLength = 0;
	_chunkSize = 0;
	_inChunk = false;
	_maxBody = 0;
}

void RequestParser::reset() {
    _state = START_LINE;
    _request = HttpRequest();
    _buffer.clear();
    _contentLength = 0;
	_chunkSize = 0;
	_inChunk = false;
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

void RequestParser::initMaxBody(const Config &config){
	_maxBody = config.getClientMaxBodySize();
}

RequestParser::ParseResult RequestParser::parse(const char* data, size_t len) throw(SourceAndRequestException) {
	if (data && (len + _buffer.size() > _maxBody) )
		throw SourceAndRequestException("Request exceeds maximum allowed size", 413);
    if(data && len > 0)
		_buffer.append(data, len);
    while (true) {
        switch (_state) {
            case START_LINE:
                if (!parseStartLine(data, len))
					return _buffer.empty() && len == 0 ? EMPTY : INCOMPLETE;
                _parseUrl();
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
	char		cr, lf;
	size_t		pos = _buffer.find("\r\n");

	if (pos == std::string::npos)
		return checkForError(data, len, false);

	std::istringstream line(_buffer);

	if (!(line >> _request.method >> _request.uri >> _request.http_version))
		throw SourceAndRequestException("Bad request", 400);
	if (!line.get(cr) || !line.get(lf) || cr != '\r' || lf != '\n')
		throw SourceAndRequestException("Bad request", 400);
	if (_request.http_version != "HTTP/1.1")
		throw SourceAndRequestException("Incorrect http version", 505);
	if (_request.uri.length() > MAX_URL_LENGTH)
		throw SourceAndRequestException("URI too long", 414);

	_buffer.erase(0, pos + 2);
	return true;
}

bool RequestParser::parseHeaders(const char *data, size_t len) throw(SourceAndRequestException) {
	size_t pos;
	while ((pos = _buffer.find("\r\n")) != std::string::npos) {
		 std::string line = _buffer.substr(0, pos);
		 _buffer.erase(0, pos + 2);
		 if (line.empty()) {
			 if (_request.headers.find("Host") == _request.headers.end() || _request.headers["Host"].empty())
			 	throw SourceAndRequestException("No Host header", 400);
			return true;
		}
		size_t colon = line.find(":");
		if (colon == std::string::npos)
			checkForError(data, len, true);
		if (colon + 1 >= line.size() || (line[colon + 1] != ' ' && line[colon + 1] != '\t'))
		throw SourceAndRequestException("Missing required whitespace after colon in header", 400);
		std::string key = WebServUtils::trim(line.substr(0, colon));
		std::string value = WebServUtils::trim(line.substr(colon + 1));
		_request.headers[key] = value;
	}
	return checkForError(data, len, false);
}

bool RequestParser::parseBody(const char *data, size_t len) throw(SourceAndRequestException) {
    if (_request.method != "POST") return true;
	std::map<std::string, std::string>::iterator it = _request.headers.find("Transfer-Encoding");
	if (it != _request.headers.end() && it->second == "chunked") {
		if (!_handleChunkedInput())
			return checkForError(data, len, false);
		_contentLength = _request.body.size();
	}
	else {
		std::map<std::string, std::string>::iterator it = _request.headers.find("Content-Length");
		if (it == _request.headers.end()) {
			throw SourceAndRequestException("No Content Length header", 411);
		}
		_contentLength = std::atoi(it->second.c_str());
		if (_contentLength > _maxBody)
			throw SourceAndRequestException("Body too big", 413);
		if (_buffer.size() < _contentLength) return checkForError(data, len, false);
		_request.body = _buffer.substr(0, _contentLength);
		_buffer.erase(0, _contentLength);
	}
    return true;
}

bool RequestParser::_handleChunkedInput(){
	Logger::debug() << "In handleChunkedInput" <<std::endl;
	std::string line;
	size_t pos;
	size_t chunkPart;

	while (!_buffer.empty()){
		if (!_inChunk) {
			if ((pos = _buffer.find("\r\n")) == std::string::npos)
				return false;

			_inChunk = true;
			_chunkPartRead = 0;
			_parseChunkSize(_buffer.substr(0, pos));
			Logger::debug() << "Chunk size is: " <<_chunkSize <<std::endl;
			_buffer.erase(0, pos + 2);
		}

		chunkPart = std::min(_buffer.length(), _chunkSize - _chunkPartRead);
		_request.body.append(_buffer.substr(0, chunkPart));
		_chunkPartRead += chunkPart;

		if (_chunkPartRead < _chunkSize || _buffer.size() < chunkPart + 2) {
				_buffer.erase(0, chunkPart);
				return false;
		}
		if (_buffer.substr(chunkPart, 2) != "\r\n")
			throw SourceAndRequestException("Missing CRLF after chunk data", 400);
		if (_chunkSize == 0) {
			Logger::debug() << "ChunkedInput done" <<std::endl;
			_state = DONE;
			return true;
		}

		_buffer.erase(0, chunkPart + 2);
		_inChunk = false;
	}
	return false;
}

void RequestParser::_parseChunkSize(const std::string& hexStr){
	Logger::detail() << "In parseChunkSize, string to parse: " <<hexStr <<std::endl;
	char *endptr = NULL;
	if (hexStr.size() == 0)
		{
			Logger::debug() << "String is empty" <<std::endl;
			throw SourceAndRequestException("Malformed chunk", 400);
		}

	for (size_t i = 0; i < hexStr.size(); ++i) {
		if (!std::isxdigit(hexStr[i])){
			Logger::debug() << "Non-hex value: " << hexStr[i] <<std::endl;
			throw (SourceAndRequestException("Malformed chunk", 400));
		}
	}
    _chunkSize = std::strtol(hexStr.c_str(), &endptr, 16);
	if (*endptr != '\0'){
		Logger::debug() << "strtol failed" <<std::endl;
		throw (SourceAndRequestException("Malformed chunk", 400));
	}

	if (_request.body.size() + _chunkSize > _maxBody){
		Logger::debug() << "chunk too big" <<std::endl;
		throw SourceAndRequestException("Body size exceeds allowed maximum", 413);
	}
	Logger::debug() << "ChunkSize parsed" <<std::endl;
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
	_request.path = WebServUtils::urlDecode(url.substr(hostEnd, pathEnd - hostEnd));

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


RequestParser::ParseState RequestParser::getParseState(){
	return _state;
}

size_t RequestParser::getMaxBody()const{
	return _maxBody;
}

RequestParser::RequestParser(const RequestParser &other){
	_state = other._state;
	_contentLength = other._contentLength;
	_chunkSize = other._chunkSize;
	_request = other._request;
	_buffer = other._buffer;
	_maxBody = other._maxBody;
}

RequestParser &RequestParser::operator=(const RequestParser &other){
	if (this != &other){
		_state = other._state;
		_contentLength = other._contentLength;
		_chunkSize = other._chunkSize;
		_request = other._request;
		_buffer = other._buffer;
		_maxBody = other._maxBody;
		}
		return *this;
}

RequestParser::~RequestParser(){}
