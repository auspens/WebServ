/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wouter <wouter@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 17:11:08 by wouter            #+#    #+#             */
/*   Updated: 2025/06/08 16:51:02 by wouter           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Config.hpp"
#include <fstream>
#include <iostream>

Config::Config() {}

Config::Config(std::string &configFile) {
	_parseConfigFile(configFile);
}

Config::~Config() {}

Config::Config(const Config &src) {
	_serverConfigs = src._serverConfigs;
}

Config &Config::operator=(const Config &src) {
	if (this != &src)
	{
		_serverConfigs = src._serverConfigs;
	}
	return *this;
}

void Config::parse(const std::string &configFile) throw(ConfigParseException) {
	_parseConfigFile(configFile);
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

bool Config::getAutoIndex(const ServerConfig &serverConfig, const Location *location) {
	if (location)
		return location->getAutoIndex();
	return serverConfig.getAutoIndex();
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
		else
			throw ConfigParseException("Unexpected keyword: " + token);
		ParseUtils::skipWhitespace(file);
	}

	file.close();
}

ServerConfig *Config::_parseServerConfig(std::ifstream &configFile) throw(ConfigParseException) {
	ServerConfig *config;

	config = new ServerConfig(*this);
	config->parse(configFile);

	return config;
}
