/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Source.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wouter <wouter@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 13:33:22 by auspensk          #+#    #+#             */
/*   Updated: 2025/06/08 16:49:15 by wouter           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Source.hpp"
#include "RedirectSource.hpp"
#include "StaticFileSource.hpp"
#include "CGISource.hpp"

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
			throw SourceException("Invalid Path");
	if (location)
		_target = WebServUtils::pathJoin(location->getRootFolder(), req.path.substr(location->getPath().size()));
	else
		_target = WebServUtils::pathJoin(serverConfig.getRootFolder(), req.path);
}

Source::SourceException::SourceException(std::string error)throw(): _error(error){}
Source::SourceException::~SourceException()throw(){}
const char *Source::SourceException::what() const throw(){
	return _error.c_str();
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
std::string Source::getLocation()const{
	return _target;
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

char *Source::readFromBuffer(){
	return _body.data() + _offset; //static cast or maybe even reinterpret cast?
}
