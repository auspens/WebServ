/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Source.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: auspensk <auspensk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 13:33:22 by auspensk          #+#    #+#             */
/*   Updated: 2025/06/10 11:31:50 by auspensk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Source.hpp"
#include "RedirectSource.hpp"
#include "StaticFileSource.hpp"
#include "CGISource.hpp"
#include "ErrorPageSource.hpp"

Source::~Source() {}

Source::Source(const ServerConfig &serverConfig, const Location *location, HttpRequest req)
	throw(SourceException)
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
			throw SourceException("Invalid Path", 403);
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

Source::SourceException::SourceException(std::string error, int code)throw(): _error(error), _code(code){}
Source::SourceException::~SourceException()throw(){}
const char *Source::SourceException::what() const throw(){
	return _error.c_str();
}
int Source::SourceException::errorCode() const throw(){
	return _code;
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

const Location *Source::_findLocation (
	const std::string &target,
	const ServerConfig &serverConfig
) {
	const std::vector<Location *> locations = serverConfig.getLocations();
	std::vector<Location *>::const_iterator it;

	for (it = locations.begin(); it != locations.end(); ++it) {
		std::string locationPath = (*it)->getPath();

		if (target.compare(0, locationPath.size(), locationPath) == 0
			&& (target.size() == locationPath.size()
			|| WebServUtils::isin("/#?", target.at(locationPath.size()))))
			return *it;
	}
	return (NULL);
}

bool Source::_isCgiRequest(const ServerConfig &serverConfig, const Location *location, const std::string &path) {
	std::vector<std::string> acceptCgi = Config::getAcceptCgi(serverConfig, location);
	for (size_t i = 0; i < acceptCgi.size(); i++) {
		if (WebServUtils::strEndsWith(path, acceptCgi[i]))
			return true;
	}
	return false;
}

bool Source::_safePath(const std::string &path) const {
	if (path.find("..") != std::string::npos ||
		path.find("//") != std::string::npos ||
		path.find("\\") != std::string::npos)
		return false;
	return true;
}

Source *Source::getNewSource(const ServerConfig &serverConfig, HttpRequest req) throw(SourceException, ChildProcessNeededException) {
	const Location *location = _findLocation(req.path, serverConfig);
	// std::cout << "Location: " << (location? location->getPath():serverConfig.getRootFolder()) << std::endl;
	// std::cout << "request path:" << req.path << std::endl;
	if (location && location->isRedirect()) {
		return new RedirectSource(serverConfig, *location, req);
	}
	if (_isCgiRequest(serverConfig, location, req.path)) {
		CGISource* ptr = new CGISource(serverConfig, location, req);
			return ptr;
	}
	return new StaticFileSource(serverConfig, location, req);
}

Source *Source::getNewErrorPageSource(const ServerConfig &serverConfig, HttpRequest req, int code) {
	const Location *location = _findLocation(req.path, serverConfig);
	return new ErrorPageSource(serverConfig, location, req, code);
}

char *Source::readFromBuffer(){
	return _body.data() + _offset; //static cast or maybe even reinterpret cast?
}
