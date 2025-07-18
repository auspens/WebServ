
#include "Source.hpp"

Source::~Source() {}

Source::Source(
	const ServerConfig &serverConfig,
	const Location *location,
	HttpRequest &req
) throw(SourceAndRequestException) :
	_serverConfig(serverConfig),
	_location(location),
	_request(req) { }

void Source::init() throw(SourceAndRequestException) {
	_bytesToSend = 0;
	_offset = 0;
	_doneReading = false;
	_doneWriting = true;
	_fd = -1;
	_writeFd = -1;
	_size = 0;
	_mime = "";
	_pollableRead = false;
	_pollableWrite = false;
	_writeWhenComplete = false;
	Logger::debug() << "Reserved for body: " << _serverConfig.getBufferSize() <<std::endl;
	_body.resize(_serverConfig.getBufferSize());
}

Source::Source(const Source &src):
		_bytesToSend(src._bytesToSend)
		,_offset(src._offset)
		,_doneReading(src._doneReading)
		,_doneWriting(src._doneWriting)
		,_fd(src._fd)
		,_writeFd(src._writeFd)
		,_size(src._size)
		,_serverConfig(src._serverConfig)
		,_location(src._location)
		,_target(src._target)
		,_mime(src._mime)
		,_request(src._request)
		,_pollableRead(src._pollableRead)
		,_pollableWrite(src._pollableRead)
		,_writeWhenComplete(src._writeWhenComplete)
		,_body(src._body) {}

Source &Source::operator=(const Source &other){
	if (this != &other){
		_bytesToSend = other._bytesToSend;
		_offset = other._offset;
		_doneReading = other._doneReading;
		_doneWriting = other._doneWriting;
		_fd = other._fd;
		_writeFd = other._writeFd;
		_size = other._size;
		_location = other._location;
		_mime = other._mime;
		_request = other._request;
		_pollableRead = other._pollableRead;
		_pollableWrite = other._pollableWrite;
		_body = other._body;
		_writeWhenComplete = other._writeWhenComplete;
	}
	return *this;
}

std::string Source::getMime()const{
	return _mime;
}

std::vector<char> const &Source::getBytesRead() const{
	return _body;
}

int Source::getFd() const{
	return _fd;
}

int Source::getWriteFd() const{
	return _writeFd;
}

int Source::getSize() const{
	return _size;
}

int Source::getStatusCode() const {
	return 200;
}

bool Source::_safePath(const std::string &path) const {
	if (path.find("..") != std::string::npos ||
		path.find("//") != std::string::npos ||
		path.find("\\") != std::string::npos)
		return false;
	return true;
}

char *Source::readFromBuffer(){
	return _body.data() + _offset;
}

bool Source::isPollableRead() const {
	return _pollableRead;
}

bool Source::isPollableWrite() const {
	return _pollableWrite;
}

bool Source::isWriteWhenComplete() const {
	return _writeWhenComplete;
}

void Source::setHeader() { //default are Content-Type and Content-Length headers
	std::string header;
	header += "HTTP/1.1 200 OK\r\n";
	header += "Content-Type: " + _mime + "\r\n";
	if (_request.isNotKeepAlive())
		header += "Connection: Keep-Alive\r\n";
	header += "Content-Length: " + WebServUtils::num_to_str(_size) + "\r\n\r\n";
	Logger::debug()<< "At setHeader" << std::endl;
	Logger::debug()<< "Header: " << std::endl << header << "header length: "<< header.length()<<std::endl;
	_body.insert(_body.begin(), header.begin(), header.end());
	_bytesToSend += header.length();
	Logger::debug() << "Bytes to send: " << _bytesToSend << std::endl;
}

void Source::writeSource() throw(SourceAndRequestException) {}

bool Source::checkTimeout(int timeout) const {
	(void)timeout;
	return false;
}

void Source::_checkAccess(std::string &target) {
	DIR *dir = opendir(Config::getRootFolder(_serverConfig, _location).c_str());
	if (!dir)
		throw(SourceAndRequestException("No root folder", 404));
	closedir(dir);
	if (access(target.c_str(), F_OK))
		throw (SourceAndRequestException("Page doesn't exist", 404));
	if (access(target.c_str(), R_OK))
		throw (SourceAndRequestException("Forbidden", 403));
}

bool Source::_checkExists(std::string &target) {
	DIR *dir = opendir(Config::getRootFolder(_serverConfig, _location).c_str());
	if (!dir)
		return false;
	closedir(dir);
	if (access(target.c_str(), F_OK))
		return false;
	return true;
}

void Source::bytesSent(int bytes) {
	_bytesToSend -= bytes;
	_offset += bytes;
}

void Source::finalizeWrite() { }
