/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConfig.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wpepping <wpepping@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 17:00:59 by wpepping          #+#    #+#             */
/*   Updated: 2025/05/15 19:36:05 by wpepping         ###   ########.fr       */
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

class ServerConfig {
public:
	ServerConfig();
	~ServerConfig();
	ServerConfig(int port, std::string host);
	ServerConfig(const ServerConfig& src);
	ServerConfig& operator=(const ServerConfig& src);

	void parse(std::ifstream &file) throw(ConfigParseException);

	int getPort() const;
	const std::string *getHost() const;
	const std::vector<std::string> &getServerNames() const;
	int getBufferSize() const;
	const std::string &getRootFolder() const;
	const std::map<int, std::string> &getErrorPages() const;
	const std::vector<Location> &getLocations() const;

private:
	int							_port;
	bool						_uploadPass;
	std::string 				_rootFolder;
	std::vector<Location>		_locations;
	std::vector<std::string>	_serverNames;
	ConfigSettings				_configSettings;

	void _parseServerNames(std::ifstream &file) throw(ConfigParseException);
	void _parsePort(std::ifstream &file) throw(ConfigParseException);
	Location _parseLocation(std::ifstream &file) throw(ConfigParseException);
	void _parseRoot(std::ifstream &file) throw(ConfigParseException);
	void _parseUploadPass(std::ifstream &file) throw(ConfigParseException);
};
