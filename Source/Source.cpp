/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Source.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: auspensk <auspensk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 13:33:22 by auspensk          #+#    #+#             */
/*   Updated: 2025/07/08 15:00:30 by auspensk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Source.hpp"
#include "SourceAndRequestException.hpp"

Source::~Source() {}

Source::Source(
	const ServerConfig &serverConfig,
	const Location *location,
	HttpRequest &req
) throw(SourceAndRequestException) :
	_serverConfig(serverConfig),
	_location(location),
	_request(req) {
		init(200);
		_methodAllowed();
}

Source::Source(
	const ServerConfig &serverConfig,
	const Location *location,
	HttpRequest &req,
	int code
) throw(SourceAndRequestException) :
	_serverConfig(serverConfig),
	_location(location),
	_request(req) {
		init(code);
}

void Source::init(int code) throw(SourceAndRequestException) {
	_bytesToSend = 0;
	_offset = 0;
	_doneReading = false;
	_doneWriting = true;
	_code = code;
	_fd = -1;
	_writeFd = -1;
	_size = 0;
	_type = STATIC;
	_mime = "";
	_pollableRead = false;
	_pollableWrite = false;
	_writeWhenComplete = false;

	_body.reserve(_serverConfig.getBufferSize());

	if (_location)
		_target = WebServUtils::pathJoin(_location->getRootFolder(), _request.path.substr(_location->getPath().size()));
	else
		_target = WebServUtils::pathJoin(_serverConfig.getRootFolder(), _request.path);
}

Source::Source(const Source &src):
		_bytesToSend(src._bytesToSend)
		,_offset(src._offset)
		,_doneReading(src._doneReading)
		,_code(src._code)
		,_fd(src._fd)
		,_size(src._size)
		,_type(src._type)
		,_serverConfig(src._serverConfig)
		,_location(src._location)
		,_mime(src._mime)
		,_request(src._request) {}

Source &Source::operator=(const Source &other){
	if (this != &other){
		_bytesToSend = other._bytesToSend;
		_offset = other._offset;
		_doneReading = other._doneReading;
		_code = other._code;
		_fd = other._fd;
		_size = other._size;
		_type = other._type;
		_location = other._location;
		_mime = other._mime;
		_request = other._request;
	}
	return *this;
}

int Source::getCode()const{
	return _code;
}
std::string Source::getMime()const{
	return _mime;
}

std::vector<char> const &Source::getBytesRead()const{
	return _body;
}

int Source::getFd()const{
	return _fd;
}

int Source::getWriteFd()const{
	return _writeFd;
}

int Source::getSize()const{
	return _size;
}

SourceType Source::getType()const{
	return _type;
}

std::string Source::getRedirectLocation()const{
	if (!_location->isRedirect())
		return std::string("");
	return _location->getRedirectPath();
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

bool Source::isPollableRead() {
	return _pollableRead;
}

bool Source::isPollableWrite() {
	return _pollableWrite;
}

bool Source::isWriteWhenComplete() {
	return _writeWhenComplete;
}

void Source::setHeader(std::string header) {
	Logger::debug()<< "At setHeader" << std::endl;
	Logger::debug() << "Body: " << std::string(_body.begin(), _body.end())<< " bytesTosend: "<< _bytesToSend<<std::endl;
	Logger::debug()<< "Header: " << header << "header length: "<< header.length()<<std::endl;
	_body.insert(_body.begin(), header.begin(), header.end());
	_bytesToSend += header.length();
	Logger::debug() << "Bytes to send: " << _bytesToSend << std::endl;
}

void Source::writeSource(){}

void Source::_methodAllowed()const{
	int allowedMethods = Config::getAcceptMethod(_serverConfig, _location);
	if ((_request.method == "POST" && !(allowedMethods & METHOD_POST))
	||(_request.method == "GET" && !(allowedMethods & METHOD_GET))
	||(_request.method == "DELETE" && !(allowedMethods & METHOD_DELETE)))
		throw SourceAndRequestException("Requested method not allowed here", 403);
}

