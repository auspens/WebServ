/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConfig.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wpepping <wpepping@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 17:00:59 by wpepping          #+#    #+#             */
/*   Updated: 2025/05/09 19:26:20 by wpepping         ###   ########.fr       */
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

#define DEFAULT_PORT 3490
#define DEFAULT_HOST "localhost"
#define READ_BUFFER 1024

class ServerConfig {
public:
	ServerConfig();
	ServerConfig(std::string host, int port);
	~ServerConfig();
	ServerConfig(const ServerConfig& src);
	ServerConfig& operator=(const ServerConfig& src);

	void parse(std::ifstream &file) throw(ConfigParseException);
	void setPort(int port) throw(ConfigParseException);
	void setPort(std::string &port) throw(ConfigParseException);

	int getPort() const;
	const std::vector<std::string> &getServerNames() const;
	int getBufferSize() const;
	const std::string &getRootFolder() const;
	const std::map<int, std::string> &getErrorPages() const;
	const std::vector<Location> &getLocations() const;

private:
	int									_port;
	std::string 						_rootFolder;
	std::vector<Location>				_locations;
	std::vector<std::string>			_serverNames;
	ConfigSettings						_configSettings;

	void _parseServerNames(std::ifstream &file) throw(ConfigParseException);
	void _parsePort(std::ifstream &file) throw(ConfigParseException);
	Location _parseLocation(std::ifstream &file) throw(ConfigParseException);
	void _parseRoot(std::ifstream &file) throw(ConfigParseException);
};
