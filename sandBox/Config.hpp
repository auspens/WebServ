/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wpepping <wpepping@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 16:57:51 by wpepping          #+#    #+#             */
/*   Updated: 2025/04/24 17:35:52 by wpepping         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
