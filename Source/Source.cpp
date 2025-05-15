/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Source.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wpepping <wpepping@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 13:33:22 by auspensk          #+#    #+#             */
/*   Updated: 2025/05/15 19:05:55 by wpepping         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Source.hpp"
#include "RedirectSource.hpp"
#include "StaticFileSource.hpp"

Source::~Source(){}
Source::Source(const std::string &target, const ServerConfig &serverConfig)
		:_bytesToSend(0)
		,_code(200)
		,_fd(-1)
		,_size(0)
		,_type(STATIC)
		,_serverConfig(serverConfig)
		,_location(defineLocation(target, serverConfig))
		,_target(_serverConfig.getRootFolder() + target)
		,_mime("mime_type_to_be_defined"){}

Source::SourceException::SourceException(std::string error)throw(): _error(error){}
Source::SourceException::~SourceException()throw(){}
const char *Source::SourceException::what() const throw(){
	return _error.c_str();
}

const Location &Source::defineLocation(const std::string &target, const ServerConfig &serverConfig){
//do location lookup here
	(void) target;
	return(serverConfig.getLocations().at(0));
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

Source *Source::getNewSource(const std::string &target, const ServerConfig &serverConfig){
	Location location = defineLocation(target, serverConfig);
	if (location.isRedirect())
		return new RedirectSource(location.getRedirectPath(), serverConfig, location.getRedirectCode());
	//here will be some logic to define if request is for CGI
	return new StaticFileSource(target, serverConfig, location);
}
