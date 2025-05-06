/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Source.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: auspensk <auspensk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 13:33:22 by auspensk          #+#    #+#             */
/*   Updated: 2025/05/06 18:12:43 by auspensk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "Source.hpp"

Source::~Source(){}
Source::Source(const std::string &target, const ServerConfig &serverConfig)
		:_code(200)
		,_fd(-1)
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
	return _bytesRead;
}
int Source::getFd()const{
	return _fd;
}

Source *Source::getNewSource(const std::string &target, const ServerConfig &serverConfig){
	Location location = defineLocation(target, serverConfig);
	if (location.getRedirect().first != 0)
		return new RedirectSource(location.getRedirect().second, serverConfig, location.getRedirect().first);
	//here will be some logic to define if request is for CGI
	return new StaticFileSource(target, serverConfig, location);
}
