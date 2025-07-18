/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wpepping <wpepping@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 16:57:51 by wpepping          #+#    #+#             */
/*   Updated: 2025/07/18 16:23:37 by wpepping         ###   ########.fr       */
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

		const std::map<int, std::string>			&getErrorPages() const;
		const std::vector<std::string>				&getIndexPages() const;
		const std::map<std::string, std::string>	&getAcceptCgi() const;
		const std::vector<ServerConfig *>			&getServerConfigs() const;

		void 				parse(const std::string &configFile) throw(ConfigParseException);
		size_t				getClientMaxBodySize() const;
		int					getAcceptMethod() const;
		bool				getAutoIndex() const;
		size_t				getBufferSize() const;
		unsigned int		getConnectionTimeout() const;
		unsigned int		getCgiTimeout() const;

		static const std::map<int, std::string>			&getErrorPages(const ServerConfig &serverConfig, const Location *location);
		static const std::vector<std::string>			&getIndexPages(const ServerConfig &serverConfig, const Location *location);
		static const std::map<std::string, std::string>	&getAcceptCgi(const ServerConfig &serverConfig, const Location *location);

		static size_t		getClientMaxBodySize(const ServerConfig &serverConfig, const Location *location);
		static int			getAcceptMethod(const ServerConfig &serverConfig, const Location *location);
		static bool			getAutoIndex(const ServerConfig &serverConfig, const Location *location);
		static int			getBufferSize(const ServerConfig &serverConfig, const Location *location);
		static std::string	getRootFolder(const ServerConfig &serverConfig, const Location *location);
		static bool			acceptsMethod(const ServerConfig &serverConfig, const Location *location, std::string method);
		static bool			acceptsMethod(const ServerConfig &serverConfig, const Location *location, int method);

	private:
		ConfigSettings				_configSettings;
		std::vector<ServerConfig *>	_serverConfigs;
		size_t						_chunkSize;
		unsigned int				_connectionTimeout;
		unsigned int				_cgiTimeout;

		void 			_parseConfigFile(const std::string &configFile) throw(ConfigParseException);
		ServerConfig	*_parseServerConfig(std::ifstream &infile) throw(ConfigParseException);
		void			_parseChunkSize(std::ifstream &infile) throw(ConfigParseException);
		void			_parseConnectionTimeout(std::ifstream &infile) throw(ConfigParseException);
		void			_parseCgiTimeout(std::ifstream &infile) throw(ConfigParseException);

		void			_validateConfig() const throw(ConfigParseException);
		void			_validateServerConfig(ServerConfig &serverConfig) const throw(ConfigParseException);
		void			_validateLocation(Location &location) const throw(ConfigParseException);
};
