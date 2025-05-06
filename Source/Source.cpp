/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Source.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: auspensk <auspensk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 13:33:22 by auspensk          #+#    #+#             */
/*   Updated: 2025/05/05 15:22:48 by auspensk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "Source.hpp"

Source::~Source(){}
Source::Source(const std::string &target, const ServerConfig &serverConfig)
		:_code(200)
		,_serverConfig(serverConfig)
		,_location(defineLocation(target, serverConfig))
		,_target(_serverConfig.getRootFolder() + target)
		,_mime("mime_type_to_be_defined")
		,_size(0){}

Source::SourceException::SourceException(std::string error)throw(): _error(error){}
Source::SourceException::~SourceException()throw(){}

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
int Source::getSize()const{
	return _size;
}
std::vector<char> const &Source::getBytesRead()const{
	return _bytesRead;
}
