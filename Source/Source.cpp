/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Source.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wouter <wouter@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 13:33:22 by auspensk          #+#    #+#             */
/*   Updated: 2025/07/12 19:11:36 by wouter           ###   ########.fr       */
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
	_request(req){
}

void Source::init() throw(SourceAndRequestException) {
	if (!Config::acceptsMethod(_serverConfig, _location, _request.method))
		throw(SourceAndRequestException("Method not allowed", 405));
	_bytesToSend = 0;
	_offset = 0;
	_doneReading = false;
	_doneWriting = true;
	_fd = -1;
	_writeFd = -1;
	_size = 0;
	_type = UNKOWN;
	_mime = "";
	_pollableRead = false;
	_pollableWrite = false;
	_writeWhenComplete = false;
	_code = 200;

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
		,_fd(src._fd)
		,_size(src._size)
		,_type(src._type)
		,_serverConfig(src._serverConfig)
		,_location(src._location)
		,_mime(src._mime)
		,_request(src._request)
		,_code(src._code) {}

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

int Source::getCode() const{
	return _code;
}
std::string Source::getMime() const{
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

SourceType Source::getType() const{
	return _type;
}

std::string Source::getRedirectLocation() const{
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

bool Source::isPollableRead() const {
	return _pollableRead;
}

bool Source::isPollableWrite() const {
	return _pollableWrite;
}

bool Source::isWriteWhenComplete() const {
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

void Source::writeSource() {}

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
