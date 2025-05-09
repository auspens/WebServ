/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigSettings.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wpepping <wpepping@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/09 16:03:27 by wpepping          #+#    #+#             */
/*   Updated: 2025/05/09 19:13:33 by wpepping         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <map>
#include <string>
#include <vector>
#include "ConfigParseException.hpp"

enum e_method {
	GET,
	POST,
	DELETE
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

	size_t								getClientMaxBodySize()	const;
	const std::map<int, std::string>	&getErrorPage()			const;
	const std::vector<std::string>		&getIndex()				const;
	const std::vector<std::string>		&getAcceptCgi()			const;
	const std::vector<e_method>			&getAcceptMethod()		const;
	bool								getAutoIndex()			const;

private:
	size_t						_clientMaxBodySize;
	std::map<int, std::string>	_errorPage;
	std::vector<std::string>	_index;
	std::vector<std::string>	_acceptCgi;
	std::vector<e_method>		_acceptMethod;
	bool						_autoIndex;
};
