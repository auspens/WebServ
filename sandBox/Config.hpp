/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wouter <wouter@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 16:57:51 by wpepping          #+#    #+#             */
/*   Updated: 2025/04/25 14:43:07 by wouter           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <string>
#include <vector>
#include "ServerConfig.hpp"

class Config {
	public:
		Config();
		Config(std::string configFile);
		~Config();
		Config(const Config& other);
  		Config& operator=(const Config& other);

		std::vector<ServerConfig> getServersConfigs();

	private:
		std::vector<ServerConfig> _serverConfigs;

		void 			_parseConfigFile(std::string configFile);
		ServerConfig	_parseServerConfig(int fd);
};
