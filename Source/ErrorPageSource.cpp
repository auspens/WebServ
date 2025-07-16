
#include "ErrorPageSource.hpp"

ErrorPageSource::ErrorPageSource
		(const ServerConfig &serverConfig,
		Location const *location,
		HttpRequest &req, int code)
		: StaticFileSource(
			serverConfig,
			location,
			req,
			""
		) {
			_code = code;
}

void ErrorPageSource::getErrorPage(int code) {
	std::map<int, std::string> errorPages = Config::getErrorPages(_serverConfig, _location);
	std::map<int, std::string>::iterator it = errorPages.find(code);

	if (it != errorPages.end()) {
		_target = it->second;

		struct stat st;
		stat(_target.c_str(), &st);
		_size = st.st_size;

		if (_size < 0)
			return generateErrorPage(code);

		defineMimeType();
		_fd = open(_target.c_str(), O_RDONLY);
	}
	else
		generateErrorPage(code);
}

void ErrorPageSource::generateErrorPage(int code) {
	std::string html = PageGenerator::generateErrorPage(code);

	_body.assign(html.begin(), html.end());
	_code = code;
	_generated = true;
	_bytesToSend = _body.size();
	_size = _body.size();
	_mime = _mimeTypes.find(".html")->second;
	_doneReading = true;
}

void ErrorPageSource::init() throw(SourceAndRequestException) {
	_body.reserve(_serverConfig.getBufferSize());
	_doneReading = false;
	_mime = "text/html";
	_offset = 0;
	_fd = -1;
	_doneReading = false;
	_doneWriting = true;
	_pollableRead = false;
	_pollableWrite = false;
	getErrorPage(_code);
	setHeader();
}

ErrorPageSource::ErrorPageSource(const ErrorPageSource &src):StaticFileSource(src){
	_code = src._code;
}

ErrorPageSource &ErrorPageSource::operator=(const ErrorPageSource &other){
	if (this != &other){
		StaticFileSource::operator=(other);
		_code = other._code;
	}
	return *this;
}

ErrorPageSource::~ErrorPageSource(){}

void ErrorPageSource::setHeader(){
	std::string						header;
	std::map<int, HTTPStatusCode>	statusCodes = StatusCodesStorage::getStatusCodes();

	header += std::string("HTTP/1.1 ") + statusCodes[_code].code + " " + statusCodes[_code].message + "\r\n";
	header += "Content-Type: " + _mime + "\r\n";
	if (_request.isKeepAlive())
		header += "Connection: Keep-Alive\r\n";
	header += "Content-Length: " + WebServUtils::num_to_str(_size) + "\r\n\r\n";
	Logger::debug()<< "At ErrorPage setHeader" << std::endl;
	Logger::debug()<< "Header: " << std::endl << header << "header length: "<< header.length() << std::endl;
	_body.insert(_body.begin(), header.begin(), header.end());
	_bytesToSend += header.length();
	Logger::debug() << "Bytes to send: " << _bytesToSend << std::endl;
}

void ErrorPageSource::readSource() throw(SourceAndRequestException) {
	if (_bytesToSend == 0 && !_generated && !_doneReading) {
		ssize_t readSize = read(_fd, _body.data(), _serverConfig.getBufferSize());
		if (readSize < 0)
			generateErrorPage(_code);
		if (readSize == 0)
 			_doneReading = true;
		_bytesToSend = readSize;
		_offset = 0;
	}
}
