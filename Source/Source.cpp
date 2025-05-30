/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Source.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wpepping <wpepping@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 13:33:22 by auspensk          #+#    #+#             */
/*   Updated: 2025/05/29 17:47:58 by wpepping         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Source.hpp"
#include "RedirectSource.hpp"
#include "StaticFileSource.hpp"
#include "CGISource.hpp"

Source::~Source(){}
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

bool Source::_isCgiRequest(const Location &location, const std::string &path) {
	for (size_t i = 0; i < location.getAcceptCgi().size(); i++) {
		if (WebServUtils::strEndsWith(path, location.getAcceptCgi()[i]))
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

Source *Source::getNewSource(const ServerConfig &serverConfig, HttpRequest req) {
	const Location *location = _findLocation(req.path, serverConfig);

	//std::cout << "Location: " << (location? location->getPath():serverConfig.getRootFolder()) << std::endl;
	if (location) {
		if (location->isRedirect()) {
			//std::cout << "This location is redirect "<< std::endl;
			return new RedirectSource(serverConfig, *location, req);
		}
		//std::cout << "This location is NOT redirect "<< std::endl;
		if (_isCgiRequest(*location, req.path)) {
			CGISource* ptr = new CGISource(serverConfig, location, req);
			if (ptr->getIfExists() == 1)
				return ptr;
			else
				delete ptr;
		}
	}

	return new StaticFileSource(serverConfig, location, req);
}

char *Source::readFromBuffer(){
	return _body.data() + _offset; //static cast or maybe even reinterpret cast?
}
