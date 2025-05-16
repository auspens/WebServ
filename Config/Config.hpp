/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wpepping <wpepping@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 16:57:51 by wpepping          #+#    #+#             */
/*   Updated: 2025/05/16 16:58:47 by wpepping         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <string>
#include <vector>
#include "ConfigParseException.hpp"
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

		void 								parse(const std::string &configFile) throw(ConfigParseException);
		const std::vector<ServerConfig *>	&getServerConfigs() const;

	private:
		ConfigSettings				_configSettings;
		std::vector<ServerConfig *>	_serverConfigs;

		void 			_parseConfigFile(const std::string &configFile) throw(ConfigParseException);
		ServerConfig	*_parseServerConfig(std::ifstream &configFile) throw(ConfigParseException);
};
