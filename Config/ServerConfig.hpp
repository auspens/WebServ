/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConfig.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wpepping <wpepping@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 17:00:59 by wpepping          #+#    #+#             */
/*   Updated: 2025/06/17 16:10:16 by wpepping         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <cstdlib>
#include <exception>
#include <limits>
#include <string>
#include <map>
#include <vector>
#include "ConfigParseException.hpp"
#include "ConfigSettings.hpp"
#include "Location.hpp"
#include "ParseUtils.hpp"
#include "WebServUtils.hpp"
#include "Constants.hpp"

class Config;

class ServerConfig {
public:
	ServerConfig();
	ServerConfig(const Config &config);
	ServerConfig(const Config &config, int port, std::string host);
	ServerConfig(const ServerConfig &src);
	~ServerConfig();
	ServerConfig& operator=(const ServerConfig& src);

	void parse(std::ifstream &file) throw(ConfigParseException);

	int									getPort() const;
	const std::vector<std::string>		&getServerNames() const;
	size_t								getBufferSize() const;
	const std::string 					&getRootFolder() const;
	const std::map<int, std::string>	&getErrorPages() const;
	const std::vector<Location *>		&getLocations() const;
	size_t								getClientMaxBodySize() const;
	const std::vector<std::string>		&getIndexPages() const;
	const std::vector<std::string>		&getAcceptCgi() const;
	int									getAcceptMethod() const;
	bool								getAutoIndex() const;

private:
	int							_port;
	std::string 				_rootFolder;
	std::vector<Location *>		_locations;
	std::vector<std::string>	_serverNames;
	ConfigSettings				_configSettings;
	const Config				*_config;

	void _parseServerNames(std::ifstream &file) throw(ConfigParseException);
	void _parsePort(std::ifstream &file) throw(ConfigParseException);
	Location *_parseLocation(std::ifstream &file) throw(ConfigParseException);
	void _parseRoot(std::ifstream &file) throw(ConfigParseException);
};
