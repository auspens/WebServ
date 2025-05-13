/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Source.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: auspensk <auspensk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 13:33:22 by auspensk          #+#    #+#             */
/*   Updated: 2025/05/13 17:44:19 by auspensk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Source.hpp"
#include "RedirectSource.hpp"
#include "StaticFileSource.hpp"
#include "CGISource.hpp"

std::map <std::string, std::string> Source::_mimeTypes;

std::string trim(const std::string& str) {
	size_t start = 0, end = str.length();
	while (start < end && std::isspace(str[start])) start++;
	while (end > start && std::isspace(str[end - 1])) end--;
	if (start < end && str[start] == '\"') ++start;
	if (end > start && str[end - 1] == '\"') --end;
	return str.substr(start, end - start);
}

struct mimeTypesInitializer {
	mimeTypesInitializer(){
		std::ifstream file(MIME_TYPES_JSON);
		if (!file)
			throw (std::runtime_error("Could not open json with mime types"));
		std::string line;

		while (std::getline(file, line)) {
			std::string::size_type colon = line.find(':');
			if (colon == std::string::npos) continue;

			std::string key = trim(line.substr(0, colon));
			std::string val = trim(line.substr(colon + 1));

			// Remove trailing comma if present
			if (!val.empty() && val[val.size() - 1] == ',')
				val = val.substr(0, val.size() - 1);
			Source::_mimeTypes[key] = val;
		}
	}
};

static mimeTypesInitializer _mimeTypeInitializer;

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
std::string Source::getLocation()const{
	return _target;
}

Source *Source::getNewSource(const std::string &target, const ServerConfig &serverConfig){
	Location location = defineLocation(target, serverConfig);
	if (location.getRedirect().first != 0)
		return new RedirectSource(location.getRedirect().second, serverConfig, location.getRedirect().first);
	if (target.find(".py") != std::string::npos)
		return new CGISource(target, serverConfig, location);
	return new StaticFileSource(target, serverConfig, location);
}
