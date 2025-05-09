/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConfig.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wpepping <wpepping@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 12:55:35 by wouter            #+#    #+#             */
/*   Updated: 2025/05/09 20:12:53 by wpepping         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ServerConfig.hpp"

ServerConfig::ServerConfig() { }

ServerConfig::~ServerConfig() {
	// TODO
}

ServerConfig::ServerConfig(const ServerConfig& src) {
	// TODO
}

ServerConfig &ServerConfig::operator=(const ServerConfig& src) {
	if (this != &src) {
		// TODO
	}
	return (*this);
}

void ServerConfig::parse(std::ifstream &file) throw(ConfigParseException) {
	std::string token;

	token = ParseUtils::parseToken(file);
	ParseUtils::expectWhitespace(file);
	if (_configSettings.isConfigSetting(token))
		_configSettings.parseConfigSetting(file, token);
	else if (token == "location")
		_locations.push_back(_parseLocation(file));
	else if (token == "server_name")
		_parseServerNames(file);
	else if (token == "listen")
		_parsePort(file);
	else if (token == "root")
		_parseRoot(file);
	else
		throw ConfigParseException("Unexpected token: " + token);
}

void ServerConfig::_parseServerNames(std::ifstream &file) throw(ConfigParseException) {
	std::string	server_name;

	ParseUtils::skipWhitespace(file);
	while (file.peek() != ';') {
		if (file.fail())
			throw ConfigParseException("Error reading from file");
		if (file.eof())
			throw ConfigParseException("Expected ';'");

		file >> server_name;
		if (server_name.length() > 255)
			throw ConfigParseException("Invalid value for server name");
		_serverNames.push_back(server_name);
	}
}

void ServerConfig::_parsePort(std::ifstream &file) throw(ConfigParseException) {
	std::string port;

	file >> port;
	_port = ParseUtils::parseInt(port, 0, 65535);
	ParseUtils::expectChar(file, ';');
}

Location ServerConfig::_parseLocation(std::ifstream &file) throw(ConfigParseException) {
	Location *location;
	std::string token;

	if (!ParseUtils::expectChar(file, '{'))
		throw ConfigParseException("Expected '{'");
	if (!ParseUtils::expectChar(file, '\n'))
		throw ConfigParseException("Expected end of line");

	location = new Location();
	location->parse(file);
	return *location;
}

void Location::_parseRoot(std::ifstream &file) throw(ConfigParseException) {
}

int ServerConfig::getPort() const {
	return _port;
}

const std::vector<std::string> &ServerConfig::getServerNames() const {
	return _serverNames;
}

const std::map<int, std::string> &ServerConfig::getErrorPages() const {
}

int ServerConfig::getBufferSize() const {
	return READ_BUFFER;
}

const std::vector<Location> &ServerConfig::getLocations() const{
	return _locations;
}

const std::string & ServerConfig::getRootFolder() const {
	return _rootFolder;
}
