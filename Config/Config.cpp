/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wpepping <wpepping@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 17:11:08 by wouter            #+#    #+#             */
/*   Updated: 2025/05/16 16:58:43 by wpepping         ###   ########.fr       */
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
		else if (token == "server"){
			_serverConfigs.push_back(_parseServerConfig(file));
			std::cout << "port in Config: " << getServerConfigs()[0]->getPort() << std::endl;}
		else
			throw ConfigParseException("Unexpected keyword: " + token);
		ParseUtils::skipWhitespace(file);
	}

	file.close();
}

ServerConfig *Config::_parseServerConfig(std::ifstream &configFile) throw(ConfigParseException) {
	ServerConfig *config;

	config = new ServerConfig();
	config->parse(configFile);

	return config;
}
