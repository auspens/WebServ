/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigSettings.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: auspensk <auspensk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/09 16:03:27 by wpepping          #+#    #+#             */
/*   Updated: 2025/06/17 11:23:03 by auspensk         ###   ########.fr       */
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
	ConfigSettings &operator=(ConfigSettings &src);

	bool isConfigSetting(std::string token) const;
	void parseConfigSetting(std::ifstream &infile, std::string token) throw(ConfigParseException);

	void parseClientMaxBodySize(std::ifstream &infile) throw(ConfigParseException);
	void parseErrorPage(std::ifstream &infile) throw(ConfigParseException);
	void parseIndex(std::ifstream &infile) throw(ConfigParseException);
	void parseAcceptCgi(std::ifstream &infile) throw(ConfigParseException);
	void parseAcceptMethod(std::ifstream &infile) throw(ConfigParseException);
	void parseAutoIndex(std::ifstream &infile) throw(ConfigParseException);

	size_t								getClientMaxBodySize()			const;
	const std::map<int, std::string>	&getErrorPages()				const;
	const std::vector<std::string>		&getIndexPages()				const;
	const std::vector<std::string>		&getAcceptCgi()					const;
	int									getAcceptMethod()				const;
	bool								getAutoIndex()					const;
	bool								autoIndexIsSet()				const;

	bool acceptsMethod(std::string method)	const;

private:
	size_t						_clientMaxBodySize;
	std::map<int, std::string>	_errorPages;
	std::vector<std::string>	_index;
	std::vector<std::string>	_acceptCgi;
	int							_acceptMethod;
	bool						_autoIndex;
	bool						_autoIndexIsSet;
};
