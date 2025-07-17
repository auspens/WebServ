/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigSettings.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wpepping <wpepping@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/09 16:03:27 by wpepping          #+#    #+#             */
/*   Updated: 2025/07/17 17:54:08 by wpepping         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <map>
#include <string>
#include <utility>
#include <vector>
#include "ConfigParseException.hpp"
#include "Constants.hpp"
#include "ParseUtils.hpp"
#include "WebServUtils.hpp"

class ConfigSettings {
public:
	ConfigSettings();
	~ConfigSettings();
	ConfigSettings(ConfigSettings &src);
	ConfigSettings &operator=(const ConfigSettings &src);

	bool isConfigSetting(std::string token) const;
	void parseConfigSetting(std::ifstream &infile, std::string token) throw(ConfigParseException);

	void parseClientMaxBodySize(std::ifstream &infile) throw(ConfigParseException);
	void parseErrorPage(std::ifstream &infile) throw(ConfigParseException);
	void parseIndex(std::ifstream &infile) throw(ConfigParseException);
	void parseAcceptCgi(std::ifstream &infile) throw(ConfigParseException);
	void parseAcceptMethod(std::ifstream &infile) throw(ConfigParseException);
	void parseAutoIndex(std::ifstream &infile) throw(ConfigParseException);

	size_t										getClientMaxBodySize()	const;
	const std::map<int, std::string>			&getErrorPages()		const;
	const std::vector<std::string>				&getIndexPages()		const;
	const std::map<std::string, std::string>	&getAcceptCgi()			const;
	int											getAcceptMethod()		const;
	bool										getAutoIndex()			const;
	bool										autoIndexIsSet()		const;

private:
	size_t								_clientMaxBodySize;
	std::map<int, std::string>			_errorPages;
	std::vector<std::string>			_index;
	std::map<std::string, std::string>	_acceptCgi;
	int									_acceptMethod;
	bool								_autoIndex;
	bool								_autoIndexIsSet;
};
