/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wpepping <wpepping@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 16:57:51 by wpepping          #+#    #+#             */
/*   Updated: 2025/05/09 14:48:54 by wpepping         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <string>
#include <vector>
#include "ConfigSettings.hpp"
#include "ParseUtils.hpp"
#include "ServerConfig.hpp"

class Config {
	public:
		Config();
		Config(std::string &configFile);
		~Config();
		Config(const Config &src);
  		Config &operator=(const Config &src);

		const std::vector<ServerConfig> &getServersConfigs() const;

	private:
		ConfigSettings _configSettings;
		std::vector<ServerConfig> _serverConfigs;

		void 			_parseConfigFile(std::string &configFile);
		ServerConfig	_parseServerConfig(std::ifstream &configFile);
};
