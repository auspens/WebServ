/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigSettings.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wouter <wouter@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/09 16:03:27 by wpepping          #+#    #+#             */
/*   Updated: 2025/05/14 17:38:53 by wouter           ###   ########.fr       */
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

enum eHttpMethod {
		METHOD_NONE   = 0,
		METHOD_GET    = 1 << 0, // 0001
		METHOD_POST   = 1 << 1, // 0010
		METHOD_DELETE = 1 << 2  // 0100
};

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

	size_t										getClientMaxBodySize()	const;
	const std::map<std::string, std::string>	&getErrorPage()			const;
	const std::vector<std::string>				&getIndex()				const;
	const std::vector<std::string>				&getAcceptCgi()			const;
	int											getAcceptMethod()		const;
	bool										getAutoIndex()			const;

	bool acceptsMethod(std::string method)	const;

private:
	size_t								_clientMaxBodySize;
	std::map<std::string, std::string>	_errorPage;
	std::vector<std::string>			_index;
	std::vector<std::string>			_acceptCgi;
	int									_acceptMethod;
	bool								_autoIndex;

	bool _isErrorCode(std::string const &str) const;
};
