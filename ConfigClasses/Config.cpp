/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: auspensk <auspensk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 17:11:08 by wouter            #+#    #+#             */
/*   Updated: 2025/05/06 12:11:22 by auspensk         ###   ########.fr       */
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

const std::vector<ServerConfig> &Config::getServersConfigs() const {
	return _serverConfigs;
}

void Config::_parseConfigFile(std::string &configFile) {
	std::ifstream file(configFile.c_str());
	if (!file.is_open())
	{
		std::cerr << "Failed to open config file: " << configFile << std::endl;
		exit(EXIT_FAILURE);
	}

	ServerConfig serverConfig = _parseServerConfig(file);
	_serverConfigs.push_back(serverConfig);

	file.close();
}

ServerConfig Config::_parseServerConfig(std::ifstream &configFile) {
	(void)configFile;
	ServerConfig config(DEFAULT_HOST, DEFAULT_PORT);
	config.setRootFolder("www");
	return config;
}
