/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConfig.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wouter <wouter@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 17:00:59 by wpepping          #+#    #+#             */
/*   Updated: 2025/04/25 14:43:15 by wouter           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <exception>
#include <string>

class ServerConfig {
public:
	class InvalidConfigValueException : std::exception {
		public:
		virtual const char* what() const throw();
	};
	ServerConfig();
	~ServerConfig();
	ServerConfig(const ServerConfig& other);
	ServerConfig& operator=(const ServerConfig& other);

	void setPort(const int port) throw(InvalidConfigValueException);
	const int getPort() const;

private:
	std::string	_host;
	int			_port;
	std::string _root_folder;
};
