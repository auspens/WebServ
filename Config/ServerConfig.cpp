/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConfig.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wpepping <wpepping@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 12:55:35 by wouter            #+#    #+#             */
/*   Updated: 2025/05/23 18:59:28 by wpepping         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Config.hpp"
#include "ServerConfig.hpp"

ServerConfig::ServerConfig() : _port(0) { }

ServerConfig::ServerConfig(const Config &config) : _port(0), _config(&config) { }

ServerConfig::ServerConfig(const Config &config, int port, std::string host) : _port(port), _config(&config) {
	_serverNames.push_back(host);
}

ServerConfig::~ServerConfig() { }

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

	ParseUtils::expectChar(infile, '{');
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
	_port = ParseUtils::parseInt(port, 1, 65535);

	ParseUtils::expectChar(infile, ';');
}

Location *ServerConfig::_parseLocation(std::ifstream &infile) throw(ConfigParseException) {
	Location *location;
	std::string token;

	location = new Location(*this);
	location->parse(infile);

	return location;
}

void ServerConfig::_parseRoot(std::ifstream &infile) throw(ConfigParseException) {
	std::string token;

	token = ParseUtils::parseValue(infile);
	ParseUtils::expectChar(infile, ';');

	if (!WebServUtils::folderExists(token))
		throw ConfigParseException("Root folder does not exist");

	_rootFolder = token;
}

void ServerConfig::_parseUploadPass(std::ifstream &infile) throw(ConfigParseException) {
	_uploadPass = 1;
	try {
		_parseRoot(infile);
	} catch (ConfigParseException &e) {
		if (std::string(e.what()) == "Root folder does not exist")
			throw ConfigParseException("Upload pass folder does not exist");
		else
			throw;
	}
}

int ServerConfig::getPort() const {
	if (_port)
		return _port;
	return DEFAULT_PORT;
}

const std::string *ServerConfig::getHost() const { // Should be removed
	#include <iostream>
	std::cout << "getHost() should be removed, use getServerNames() instead" << std::endl;

	if (_serverNames.size() > 0)
		return &_serverNames[0];
	else
		return NULL;
}

const std::vector<std::string> &ServerConfig::getServerNames() const {
	return _serverNames;
}

int ServerConfig::getBufferSize() const {
	return READ_BUFFER;
}

const std::vector<Location *> &ServerConfig::getLocations() const{
	return _locations;
}

const std::string &ServerConfig::getRootFolder() const {
	return _rootFolder;
}

size_t ServerConfig::getClientMaxBodySize() const {
	if (_configSettings.getClientMaxBodySize())
		return _configSettings.getClientMaxBodySize();
	if (_config)
		return _config->getClientMaxBodySize();
	return DEFAULT_CLIENT_MAX_BODY_SIZE;
}

const std::map<int, std::string> &ServerConfig::getErrorPages() const {
	if (!_configSettings.getErrorPages().empty())
		return _configSettings.getErrorPages();
	if (_config)
		return _config->getErrorPages();
	return _configSettings.getErrorPages();
}

const std::vector<std::string> &ServerConfig::getIndexPages() const {
	if (!_configSettings.getIndexPages().empty())
		return _configSettings.getIndexPages();
	if (_config)
		return _config->getIndexPages();
	return _configSettings.getIndexPages();
}

const std::vector<std::string> &ServerConfig::getAcceptCgi() const {
	if (!_configSettings.getAcceptCgi().empty())
		return _configSettings.getAcceptCgi();
	if (_config)
		return _config->getAcceptCgi();
	return _configSettings.getAcceptCgi();
}

int ServerConfig::getAcceptMethod() const {
	if (_configSettings.getAcceptMethod())
		return _configSettings.getAcceptMethod();
	if (_config)
		return _config->getAcceptMethod();
	return DEFAULT_ACCEPT_METHOD;
}

bool ServerConfig::getAutoIndex() const {
	if (_configSettings.autoIndexIsSet())
		return _configSettings.getAutoIndex();
	if (_config)
		return _config->getAutoIndex();
	return DEFAULT_AUTO_INDEX;
}
