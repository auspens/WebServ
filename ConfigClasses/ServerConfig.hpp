/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConfig.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: auspensk <auspensk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 17:00:59 by wpepping          #+#    #+#             */
/*   Updated: 2025/05/05 15:12:48 by auspensk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <cstdlib>
#include <exception>
#include <limits>
#include <string>
#include <map>
#include <vector>
#include "Location.hpp"

#define DEFAULT_PORT 3490
#define DEFAULT_HOST "localhost"
#define READ_BUFFER 1024

class ServerConfig {
public:
	class InvalidConfigValueException : std::exception {
		public:
		virtual const char* what() const throw();
	};

	ServerConfig();
	ServerConfig(std::string host, int port);
	~ServerConfig();
	ServerConfig(const ServerConfig& src);
	ServerConfig& operator=(const ServerConfig& src);

	void setPort(int port) throw(InvalidConfigValueException);
	void setPort(std::string &port) throw(InvalidConfigValueException);
	void setHost(std::string &host) throw(InvalidConfigValueException);
	void setRootFolder(const std::string &rootFolder)throw(InvalidConfigValueException);

	int getPort() const;
	const std::string &getHost() const;
	int getBufferSize() const;
	const std::string &getRootFolder()const;
	const std::map<int, std::string> &getErrorPages()const;
	const std::vector<Location> &getLocations()const;

private:
	std::string							_host;
	int									_port;
	std::string 						_rootFolder;
	std::map<int, std::string>			_errorPages;
	std::vector<Location>				_locations;
};
