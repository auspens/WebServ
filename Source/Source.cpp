/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Source.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: auspensk <auspensk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 13:33:22 by auspensk          #+#    #+#             */
/*   Updated: 2025/04/30 16:44:26 by auspensk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "Source.hpp"

Source::~Source(){}
Source::Source(const std::string &target, const ServerConfig &serverConfig)
		:_fd(0)
		,_code(200)
		,_location(defineLocation(target, serverConfig))
		,_serverConfig(serverConfig)
		,_target(_serverConfig.getRootFolder() + target){}

Source::SourceException::SourceException(std::string error): _error(error){}

const Location &Source::defineLocation(const std::string &target, const ServerConfig &serverConfig){
//do location lookup here
	return(serverConfig.getLocations().at(0));
}
