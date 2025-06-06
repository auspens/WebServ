/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Source.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: auspensk <auspensk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 13:33:22 by auspensk          #+#    #+#             */
/*   Updated: 2025/06/06 15:35:48 by auspensk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Source.hpp"
#include "RedirectSource.hpp"
#include "StaticFileSource.hpp"
#include "CGISource.hpp"

Source::~Source(){}
Source::Source(const ServerConfig &serverConfig, const Location *location, HttpRequest req)
		:_bytesToSend(0)
		,_offset(0)
		,_doneReading(false)
		,_code(200)
		,_fd(-1)
		,_size(0)
		,_type(STATIC)
		,_serverConfig(serverConfig)
		,_location(location)
		,_target(WebServUtils::pathJoin(serverConfig.getRootFolder(), req.path))
		,_mime("")
		,_request(req){}

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
	const std::vector<std::string> *acceptCgi;

	if (location)
		acceptCgi = &location->getAcceptCgi();
	else
		acceptCgi = &serverConfig.getAcceptCgi();

	for (size_t i = 0; i < acceptCgi->size(); i++) {
		if (WebServUtils::strEndsWith(path, acceptCgi->at(i)))
			return true;
	}
	return false;
}

Source *Source::getNewSource(const ServerConfig &serverConfig, HttpRequest req) {
	const Location *location = _findLocation(req.path, serverConfig);

	std::cout << "Location: " << (location? location->getPath():serverConfig.getRootFolder()) << std::endl;
	std::cout << "request path:" << req.path << std::endl;
	if (location && location->isRedirect()) {
		//std::cout << "This location is redirect "<< std::endl;
		return new RedirectSource(serverConfig, *location, req);
	}
	if (_isCgiRequest(serverConfig, location, req.path)) {
		CGISource* ptr = new CGISource(serverConfig, location, req);
		if (ptr->getIfExists() == 1)
			return ptr;
		else
			delete ptr;
	}
	std::cout << "Static file requested"<< std::endl;
	return new StaticFileSource(serverConfig, location, req);
}

char *Source::readFromBuffer(){
	return _body.data() + _offset; //static cast or maybe even reinterpret cast?
}
