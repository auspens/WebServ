/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wouter <wouter@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 17:11:08 by wouter            #+#    #+#             */
/*   Updated: 2025/07/09 21:26:06 by wouter           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Config.hpp"
#include <fstream>
#include <iostream>

Config::Config() : _chunkSize(0), _connectionTimeout(0) {}

Config::Config(std::string &configFile) : _chunkSize(0), _connectionTimeout(0) {
	_parseConfigFile(configFile);
}

Config::~Config() {
	std::vector<ServerConfig*>::iterator it;

	for (it = _serverConfigs.begin(); it != _serverConfigs.end(); ++it)
		delete *it;
}

Config::Config(const Config &src) {
	_serverConfigs = src._serverConfigs;
	_chunkSize = src._chunkSize;
}

Config &Config::operator=(const Config &src) {
	if (this != &src) {
		_chunkSize = src._chunkSize;
		_serverConfigs = src._serverConfigs;
	}
	return *this;
}

void Config::parse(const std::string &configFile) throw(ConfigParseException) {
	_parseConfigFile(configFile);
	_validateConfig();
}

const std::vector<ServerConfig *> &Config::getServerConfigs() const {
	return _serverConfigs;
}

size_t Config::getClientMaxBodySize() const {
	if (_configSettings.getClientMaxBodySize())
		return _configSettings.getClientMaxBodySize();
	return DEFAULT_CLIENT_MAX_BODY_SIZE;
}

const std::map<int, std::string>& Config::getErrorPages() const {
	return _configSettings.getErrorPages();
}

const std::vector<std::string>& Config::getIndexPages() const {
	return _configSettings.getIndexPages();
}

const std::vector<std::string>& Config::getAcceptCgi() const {
	return _configSettings.getAcceptCgi();
}

int Config::getAcceptMethod() const {
	if (_configSettings.getAcceptMethod())
		return _configSettings.getAcceptMethod();
	return DEFAULT_ACCEPT_METHOD;
}

bool Config::getAutoIndex() const {
	if (_configSettings.autoIndexIsSet())
		return _configSettings.getAutoIndex();
	return DEFAULT_AUTO_INDEX;
}

size_t Config::getBufferSize() const {
	if (_chunkSize)
		return _chunkSize;
	return DEFAULT_CHUNK_SIZE;
}

unsigned int Config::getConnectionTimeout() const {
	if (_connectionTimeout)
		return _connectionTimeout;
	return DEFAULT_CONNECTION_TIMEOUT;
}

size_t Config::getClientMaxBodySize(const ServerConfig &serverConfig, const Location *location) {
	if (location)
		return location->getClientMaxBodySize();
	return serverConfig.getClientMaxBodySize();
}

const std::map<int, std::string> &Config::getErrorPages(const ServerConfig &serverConfig, const Location *location) {
	if (location)
		return location->getErrorPages();
	return serverConfig.getErrorPages();
}

const std::vector<std::string> &Config::getIndexPages(const ServerConfig &serverConfig, const Location *location) {
	if (location)
		return location->getIndexPages();
	return serverConfig.getIndexPages();
}

const std::vector<std::string> &Config::getAcceptCgi(const ServerConfig &serverConfig, const Location *location) {
	if (location)
		return location->getAcceptCgi();
	return serverConfig.getAcceptCgi();
}

int Config::getAcceptMethod(const ServerConfig &serverConfig, const Location *location) {
	if (location)
		return location->getAcceptMethod();
	return serverConfig.getAcceptMethod();
}

bool Config::acceptsMethod(const ServerConfig &serverConfig, const Location *location, std::string method) {
	int acceptMethod = getAcceptMethod(serverConfig, location);

	if (method == "GET")
		return acceptMethod & METHOD_GET;
	if (method == "POST")
		return acceptMethod & METHOD_POST;
	if (method == "DELETE")
		return acceptMethod & METHOD_DELETE;
	return false;
}

bool Config::acceptsMethod(const ServerConfig &serverConfig, const Location *location, int method) {
	return getAcceptMethod(serverConfig, location) & method;
}

bool Config::getAutoIndex(const ServerConfig &serverConfig, const Location *location) {
	if (location)
		return location->getAutoIndex();
	return serverConfig.getAutoIndex();
}

std::string Config::getRootFolder(const ServerConfig &serverConfig, const Location *location) {
	if (location)
		return location->getRootFolder();
	return serverConfig.getRootFolder();
}

void Config::_parseConfigFile(const std::string &configFile) throw(ConfigParseException) {
	std::string token;
	std::ifstream file(configFile.c_str());

	if (!file.is_open())
		throw ConfigParseException("Failed to open config file: " + configFile);

	file.peek();
	while (!file.eof()) {
		token = ParseUtils::parseToken(file);
		if (file.fail())
			throw ConfigParseException("Setting name expected");
		ParseUtils::expectWhitespace(file);
		if (_configSettings.isConfigSetting(token))
			_configSettings.parseConfigSetting(file, token);
		else if (token == "server")
			_serverConfigs.push_back(_parseServerConfig(file));
		else if (token == "chunk_size")
			_parseChunkSize(file);
		else if (token == "connection_timeout")
			_parseConnectionTimeout(file);
		else
			throw ConfigParseException("Unexpected keyword: " + token);
		ParseUtils::skipWhitespace(file);
	}

	file.close();
}

void Config::_validateConfig() const throw(ConfigParseException) {
	if (_serverConfigs.empty())
		throw ConfigParseException("No servers configured");
	if (getAcceptMethod() & METHOD_DELETE)
		throw ConfigParseException("DELETE method only allowed for upload pass");

	for (size_t i = 0; i < _serverConfigs.size(); i++)
		_validateServerConfig(*_serverConfigs[i]);
}

void Config::_validateServerConfig(ServerConfig &serverConfig) const throw(ConfigParseException) {
	std::vector<Location *> locations = serverConfig.getLocations();

	if (!serverConfig.getPort())
		throw ConfigParseException("Missing listen port for server");
	if (serverConfig.getRootFolder() == "" && locations.empty())
		throw ConfigParseException("Missing root folder and no locations configured for server");
	if (getAcceptMethod() & METHOD_DELETE)
		throw ConfigParseException("DELETE method only allowed for upload pass");

	for (size_t i = 0; i < locations.size(); i++)
		_validateLocation(*locations[i]);
}

void Config::_validateLocation(Location &location) const throw(ConfigParseException) {
	if (location.getRootFolder() == "")
		throw ConfigParseException("Missing root folder for location: " + location.getPath());
	if (getAcceptMethod() & METHOD_DELETE)
		throw ConfigParseException("DELETE method only allowed for upload pass");
}

ServerConfig *Config::_parseServerConfig(std::ifstream &infile) throw(ConfigParseException) {
	ServerConfig *config;

	config = new ServerConfig(*this);
	config->parse(infile);

	return config;
}

void Config::_parseChunkSize(std::ifstream &infile) throw(ConfigParseException) {
	std::string chunkSize;

	chunkSize = ParseUtils::parseValue(infile);
	_chunkSize = ParseUtils::parseLong(chunkSize, 1, std::numeric_limits<long>::max());

	ParseUtils::expectChar(infile, ';');
}

void Config::_parseConnectionTimeout(std::ifstream &infile) throw(ConfigParseException) {
	std::string timeout;

	timeout = ParseUtils::parseValue(infile);
	_connectionTimeout = ParseUtils::parseLong(timeout, 1, std::numeric_limits<int>::max());

	ParseUtils::expectChar(infile, ';');
}
