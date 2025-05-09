/* ************************************************************************** */
/*																			*/
/*														:::	  ::::::::   */
/*   ConfigSettings.cpp								 :+:	  :+:	:+:   */
/*													+:+ +:+		 +:+	 */
/*   By: wpepping <wpepping@student.42berlin.de>	+#+  +:+	   +#+		*/
/*												+#+#+#+#+#+   +#+		   */
/*   Created: 2025/05/09 16:03:34 by wpepping		  #+#	#+#			 */
/*   Updated: 2025/05/09 19:16:27 by wpepping		 ###   ########.fr	   */
/*																			*/
/* ************************************************************************** */

#include "ConfigSettings.hpp"

ConfigSettings::ConfigSettings() : _clientMaxBodySize(-1), _autoIndex(false) {}

ConfigSettings::~ConfigSettings() {}

ConfigSettings::ConfigSettings(ConfigSettings &src) {
	_clientMaxBodySize = src._clientMaxBodySize;
	_errorPage = src._errorPage;
	_index = src._index;
	_acceptCgi = src._acceptCgi;
	_acceptMethod = src._acceptMethod;
	_autoIndex = src._autoIndex;
}

ConfigSettings& ConfigSettings::operator=(ConfigSettings &src) {
	if (this != &src) {
		_clientMaxBodySize = src._clientMaxBodySize;
		_errorPage = src._errorPage;
		_index = src._index;
		_acceptCgi = src._acceptCgi;
		_acceptMethod = src._acceptMethod;
		_autoIndex = src._autoIndex;
	}
	return *this;
}

bool ConfigSettings::isConfigSetting(std::string token) const {
	return token == "accept"
		|| token == "accept_cgi"
		|| token == "autoindex"
		|| token == "client_max_body_size"
		|| token == "error_page"
		|| token == "index";
}

void ConfigSettings::parseConfigSetting(std::ifstream &infile, std::string token) throw(ConfigParseException) {
	if (token == "client_max_body_size")
		parseClientMaxBodySize(infile);
	else if (token == "error_page")
		parseErrorPage(infile);
	else if (token == "index")
		parseIndex(infile);
	else if (token == "accept_cgi")
		parseAcceptCgi(infile);
	else if (token == "accept")
		parseAcceptMethod(infile);
	else if (token == "autoindex")
		parseAutoIndex(infile);
	else
		throw ConfigParseException("Unexpected token: " + token);
}

size_t ConfigSettings::getClientMaxBodySize() const {
	return _clientMaxBodySize;
}

const std::map<int, std::string>& ConfigSettings::getErrorPage() const {
	return _errorPage;
}

const std::vector<std::string>& ConfigSettings::getIndex() const {
	return _index;
}

const std::vector<std::string>& ConfigSettings::getAcceptCgi() const {
	return _acceptCgi;
}

const std::vector<e_method>& ConfigSettings::getAcceptMethod() const {
	return _acceptMethod;
}

bool ConfigSettings::getAutoIndex() const {
	return _autoIndex;
}

void ConfigSettings::parseClientMaxBodySize(std::ifstream &infile) {

}

void ConfigSettings::parseErrorPage(std::ifstream &infile) {

}

void ConfigSettings::parseIndex(std::ifstream &infile) {

}

void ConfigSettings::parseAcceptCgi(std::ifstream &infile) {

}

void ConfigSettings::parseAcceptMethod(std::ifstream &infile) {

}

void ConfigSettings::parseAutoIndex(std::ifstream &infile) {

}
