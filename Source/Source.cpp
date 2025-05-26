/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Source.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: auspensk <auspensk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 13:33:22 by auspensk          #+#    #+#             */
/*   Updated: 2025/05/26 16:56:27 by auspensk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Source.hpp"
#include "RedirectSource.hpp"
#include "StaticFileSource.hpp"
#include "CGISource.hpp"

Source::~Source(){}
Source::Source(const std::string &target, const ServerConfig &serverConfig)
		:_bytesToSend(0)
		,_offset(0)
		,_code(200)
		,_fd(-1)
		,_size(0)
		,_type(STATIC)
		,_serverConfig(serverConfig)
		,_location(defineLocation(target, serverConfig))
		,_target(_serverConfig.getRootFolder() + target)
		,_mime("")
		,_readPerformed(false){}

Source::SourceException::SourceException(std::string error)throw(): _error(error){}
Source::SourceException::~SourceException()throw(){}
const char *Source::SourceException::what() const throw(){
	return _error.c_str();
}

const Location *Source::defineLocation(const std::string &target, const ServerConfig &serverConfig){
	const std::vector<Location *> locations = serverConfig.getLocations();
	std::vector<Location *>::const_iterator it = locations.begin();
	for (;it != locations.end();++it){
		std::string locationPath = (*it)->getPath();
		if (target.find(locationPath) == 0
			&& (target.size() == locationPath.size() || target.at(locationPath.size()) == '/'))
			return *it;
	}
	return (NULL);
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

bool Source::isReadPerformed()const{
	return _readPerformed;
}

void Source::unsetReadPerformed(){
	_readPerformed = false;
}

Source *Source::getNewSource(const std::string &target, const ServerConfig &serverConfig) {
	const Location *location = defineLocation(target, serverConfig);
	if (location && location->isRedirect())
		return new RedirectSource(location->getRedirectPath(), serverConfig, location->getRedirectCode());
	if (target.find(".py") != std::string::npos)
		return new CGISource(target, serverConfig, location);
	return new StaticFileSource(target, serverConfig, location);
}

char *Source::readFromBuffer(){
	return _body.data() + _offset;
}
