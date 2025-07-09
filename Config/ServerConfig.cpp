/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConfig.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wouter <wouter@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 12:55:35 by wouter            #+#    #+#             */
/*   Updated: 2025/07/09 21:27:24 by wouter           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Config.hpp"
#include "ServerConfig.hpp"

ServerConfig::ServerConfig() { }

ServerConfig::ServerConfig(const Config &config) : _port(0), _config(&config) { }

ServerConfig::ServerConfig(const Config &config, int port, std::string host) : _port(port), _config(&config) {
	_serverNames.push_back(host);
}

ServerConfig::~ServerConfig() {
	std::vector<Location*>::iterator it;

	for (it = _locations.begin(); it != _locations.end(); ++it)
		delete *it;
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
		throw ConfigParseException("Empty value for server name");

	while (server_name != "") {
		if (server_name.length() > 255)
			throw ConfigParseException("Value for server name too long: " + server_name);
		_serverNames.push_back(server_name);
		server_name = ParseUtils::parseValue(infile);
	}

	ParseUtils::expectChar(infile, ';');
}

void ServerConfig::_parsePort(std::ifstream &infile) throw(ConfigParseException) {
	std::string port;

	port = ParseUtils::parseValue(infile);
	_port = ParseUtils::parseLong(port, 1, 65535);

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

	if (_rootFolder != "")
		throw ConfigParseException("Server config: Root folder already set");
	else if (!WebServUtils::folderExists(token))
		throw ConfigParseException("Root folder does not exist");

	_rootFolder = token;
}

int ServerConfig::getPort() const {
	if (_port)
		return _port;
	return DEFAULT_PORT;
}

const std::vector<std::string> &ServerConfig::getServerNames() const {
	return _serverNames;
}

size_t ServerConfig::getBufferSize() const {
	if (_config->getBufferSize())
		return _config->getBufferSize();
	return DEFAULT_CHUNK_SIZE;
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
