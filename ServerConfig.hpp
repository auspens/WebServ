/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConfig.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: auspensk <auspensk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 17:00:59 by wpepping          #+#    #+#             */
/*   Updated: 2025/04/28 13:55:26 by auspensk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <cstdlib>
#include <exception>
#include <limits>
#include <string>

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

	int getPort() const;
	const std::string &getHost() const;
	int getBufferSize() const;

private:
	std::string	_host;
	int			_port;
	std::string _root_folder;
};
