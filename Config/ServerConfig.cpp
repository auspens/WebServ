/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConfig.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wpepping <wpepping@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 12:55:35 by wouter            #+#    #+#             */
/*   Updated: 2025/05/15 15:30:19 by wpepping         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ServerConfig.hpp"

ServerConfig::ServerConfig() { }

ServerConfig::ServerConfig(int port, std::string host) : _port(port) {
	_serverNames.push_back(host);
}

ServerConfig::~ServerConfig() {
	// TODO
}

ServerConfig::ServerConfig(const ServerConfig& src) {
	// TODO
	(void)src;
}

ServerConfig &ServerConfig::operator=(const ServerConfig& src) {
	if (this != &src) {
		// TODO
	}
	return (*this);
}

void ServerConfig::parse(std::ifstream &infile) throw(ConfigParseException) {
	std::string token;

	while (infile.peek() != '}' && !infile.eof()) {
		token = ParseUtils::parseToken(infile);
		if (_configSettings.isConfigSetting(token))
			_configSettings.parseConfigSetting(infile, token);
		else if (token == "location")
			_locations.push_back(_parseLocation(infile));
		else if (token == "server_name")
			_parseServerNames(infile);
		else if (token == "listen")
			_parsePort(infile);
		else if (token == "root")
			_parseRoot(infile);
		else
			throw ConfigParseException("Unexpected token: " + token);
		ParseUtils::skipWhitespace(infile);
	}
	ParseUtils::expectChar(infile, '}');
}

void ServerConfig::_parseServerNames(std::ifstream &infile) throw(ConfigParseException) {
	std::string	server_name;

	server_name = ParseUtils::parseValue(infile);
	if (server_name == "")
		throw ConfigParseException("Invalid value for server name");

	while (server_name != "") {
		if (server_name.length() > 255)
			throw ConfigParseException("Invalid value for server name");
		_serverNames.push_back(server_name);
		server_name = ParseUtils::parseValue(infile);
	}

	ParseUtils::expectChar(infile, ';');
}

void ServerConfig::_parsePort(std::ifstream &infile) throw(ConfigParseException) {
	std::string port;

	port = ParseUtils::parseValue(infile);
	_port = ParseUtils::parseInt(port, 0, 65535);

	ParseUtils::expectChar(infile, ';');
}

Location ServerConfig::_parseLocation(std::ifstream &infile) throw(ConfigParseException) {
	Location *location;
	std::string token;

	location = new Location();
	location->parse(infile);

	return *location;
}

void ServerConfig::_parseRoot(std::ifstream &infile) throw(ConfigParseException) {
	std::string token;

	token = ParseUtils::parseValue(infile);
	ParseUtils::expectChar(infile, ';');

	if (!WebServUtils::folderExists(token))
		throw ConfigParseException("Root folder does not exist");

	_rootFolder = token;
}

int ServerConfig::getPort() const {
	return _port;
}

const std::string &ServerConfig::getHost() const { // Should be removed
	#include <iostream>
	std::cout << "getHost() should be removed, use getServerNames instead" << std::endl;

	return _serverNames[0];
}

const std::vector<std::string> &ServerConfig::getServerNames() const {
	return _serverNames;
}

const std::map<int, std::string> &ServerConfig::getErrorPages() const {
	return _configSettings.getErrorPages();
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
