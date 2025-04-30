/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConfig.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: auspensk <auspensk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 12:55:35 by wouter            #+#    #+#             */
/*   Updated: 2025/04/30 13:48:28 by auspensk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ServerConfig.hpp"

ServerConfig::ServerConfig() { }

ServerConfig::ServerConfig(std::string host, int port) : _host(host), _port(port) {
	//this is placeholder
	std::string path("/");
	_locations[0] = Location(path);
}

ServerConfig::~ServerConfig() { }

ServerConfig::ServerConfig(const ServerConfig& src) {
	this->_port = src._port;
	this->_host = src._host;
}

ServerConfig& ServerConfig::operator=(const ServerConfig& src) {
	if (this != &src) {
		_port = src._port;
		_host = src._host;
	}
	return (*this);
}

void ServerConfig::setPort(int port) throw(InvalidConfigValueException) {
	if (port < 0 || port > 65535)
		throw InvalidConfigValueException();
	_port = port;
}

void ServerConfig::setPort(std::string &port) throw(InvalidConfigValueException) {
	const char	*cStr;
	char		*strEnd;
	long		l;

	cStr = port.c_str();
	l = strtol(cStr, &strEnd, 10);
	if (cStr == strEnd || l < 0 || l > 65535)
		throw InvalidConfigValueException();
	_port = l;
}

void ServerConfig::setHost(std::string &host) throw(InvalidConfigValueException) {
	if (host == "")
		throw InvalidConfigValueException();
	if (host.length() > 255)
		throw InvalidConfigValueException();
	_host = host;
}

int ServerConfig::getPort() const {
	return _port;
}

const std::string &ServerConfig::getHost() const {
	return _host;
}

int ServerConfig::getBufferSize() const {
	return READ_BUFFER;
}

const char* ServerConfig::InvalidConfigValueException::what() const throw() {
	return "Invalid config value exception";
}

const std::string & ServerConfig::getRootFolder()const {
	return _root_folder;
}


const std::map<int, std::string> &ServerConfig:: getErrorPages()const{
	return _errorPages;
}

const std::vector<Location> &ServerConfig::getLocations()const{
	return _locations;
}
