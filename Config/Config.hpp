/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wpepping <wpepping@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 16:57:51 by wpepping          #+#    #+#             */
/*   Updated: 2025/06/17 16:17:09 by wpepping         ###   ########.fr       */
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
		size_t								getClientMaxBodySize() const;
		const std::map<int, std::string>	&getErrorPages() const;
		const std::vector<std::string>		&getIndexPages() const;
		const std::vector<std::string>		&getAcceptCgi() const;
		int									getAcceptMethod() const;
		bool								getAutoIndex() const;
		size_t								getBufferSize() const;

		static size_t							getClientMaxBodySize(const ServerConfig &serverConfig, const Location *location);
		static const std::map<int, std::string>	&getErrorPages(const ServerConfig &serverConfig, const Location *location);
		static const std::vector<std::string>	&getIndexPages(const ServerConfig &serverConfig, const Location *location);
		static const std::vector<std::string>	&getAcceptCgi(const ServerConfig &serverConfig, const Location *location);
		static int								getAcceptMethod(const ServerConfig &serverConfig, const Location *location);
		static bool								getAutoIndex(const ServerConfig &serverConfig, const Location *location);
		static int								getBufferSize(const ServerConfig &serverConfig, const Location *location);

	private:
		ConfigSettings				_configSettings;
		std::vector<ServerConfig *>	_serverConfigs;
		size_t						_chunkSize;

		void 			_parseConfigFile(const std::string &configFile) throw(ConfigParseException);
		ServerConfig	*_parseServerConfig(std::ifstream &configFile) throw(ConfigParseException);
		void			_parseChunkSize(std::ifstream &configFile) throw(ConfigParseException);
		void			_validateConfig() const throw(ConfigParseException);
		void			_validateServerConfig(ServerConfig &serverConfig) const throw(ConfigParseException);
		void			_validateLocation(Location &location) const throw(ConfigParseException);
};
