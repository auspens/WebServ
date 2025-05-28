/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Source.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wouter <wouter@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 13:33:22 by auspensk          #+#    #+#             */
/*   Updated: 2025/05/28 16:15:01 by wouter           ###   ########.fr       */
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

Source *Source::getNewSource(
	const ServerConfig &serverConfig,
	const Location *location,
	HttpRequest req)
{
	//std::cout << "Location: " << (location? location->getPath():serverConfig.getRootFolder()) << std::endl;
	if (location && location->isRedirect()){
		//std::cout << "This location is redirect "<< std::endl;
		return new RedirectSource(serverConfig, *location, req);
	}
	//std::cout << "This location is NOT redirect "<< std::endl;
	if (req.path.find(".py") != std::string::npos) {
		CGISource* ptr = new CGISource(serverConfig, location, req);
		if (ptr->getIfExists() == 1)
			return ptr;
		else
			delete ptr;
	}
	return new StaticFileSource(serverConfig, location, req);
}

char *Source::readFromBuffer(){
	return _body.data() + _offset; //static cast or maybe even reinterpret cast?
}
