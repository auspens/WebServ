/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Source.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: auspensk <auspensk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 13:33:22 by auspensk          #+#    #+#             */
/*   Updated: 2025/06/11 16:08:39 by auspensk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Source.hpp"
#include "SourceAndRequestException.hpp"

Source::~Source() {}

Source::Source(const ServerConfig &serverConfig, const Location *location, HttpRequest req)
	throw(SourceAndRequestException)
		:_bytesToSend(0)
		,_offset(0)
		,_doneReading(false)
		,_code(200)
		,_fd(-1)
		,_size(0)
		,_type(STATIC)
		,_serverConfig(serverConfig)
		,_location(location)
		,_mime("")
		,_request(req) {
	if (!_safePath(req.path))
			throw SourceAndRequestException("Invalid Path", 403);
	if (location)
		_target = WebServUtils::pathJoin(location->getRootFolder(), req.path.substr(location->getPath().size()));
	else
		_target = WebServUtils::pathJoin(serverConfig.getRootFolder(), req.path);
}

Source::Source(const ServerConfig &serverConfig, const Location *location, HttpRequest req, int code)
	throw()
		:_bytesToSend(0)
		,_offset(0)
		,_doneReading(false)
		,_code(code)
		,_fd(-1)
		,_size(0)
		,_type(STATIC)
		,_serverConfig(serverConfig)
		,_location(location)
		,_mime("")
		,_request(req) {
	if (location)
		_target = WebServUtils::pathJoin(location->getRootFolder(), req.path.substr(location->getPath().size()));
	else
		_target = WebServUtils::pathJoin(serverConfig.getRootFolder(), req.path);
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
	return _body.data() + _offset; //static cast or maybe even reinterpret cast?
}
