/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigSettings.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wouter <wouter@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 20:05:23 by wpepping          #+#    #+#             */
/*   Updated: 2025/07/12 17:20:04 by wouter           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ConfigSettings.hpp"

ConfigSettings::ConfigSettings() : _clientMaxBodySize(0), _acceptMethod(0), _autoIndex(false), _autoIndexIsSet(false) {}

ConfigSettings::~ConfigSettings() {}

ConfigSettings::ConfigSettings(ConfigSettings &src) {
	_clientMaxBodySize = src._clientMaxBodySize;
	_errorPages = src._errorPages;
	_index = src._index;
	_acceptCgi = src._acceptCgi;
	_acceptMethod = src._acceptMethod;
	_autoIndex = src._autoIndex;
	_autoIndexIsSet = src._autoIndexIsSet;
}

ConfigSettings& ConfigSettings::operator=(ConfigSettings &src) {
	if (this != &src) {
		_clientMaxBodySize = src._clientMaxBodySize;
		_errorPages = src._errorPages;
		_index = src._index;
		_acceptCgi = src._acceptCgi;
		_acceptMethod = src._acceptMethod;
		_autoIndex = src._autoIndex;
		_autoIndexIsSet = src._autoIndexIsSet;
	}
	return *this;
}

bool ConfigSettings::isConfigSetting(std::string token) const {
	return token == "accept"
		|| token == "accept_cgi"
		|| token == "autoindex"
		|| token == "client_max_request_size"
		|| token == "error_page"
		|| token == "index";
}

void ConfigSettings::parseConfigSetting(std::ifstream &infile, std::string token) throw(ConfigParseException) {
	if (token == "client_max_request_size")
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

const std::map<int, std::string>& ConfigSettings::getErrorPages() const {
	return _errorPages;
}

const std::vector<std::string>& ConfigSettings::getIndexPages() const {
	return _index;
}

const std::vector<std::string>& ConfigSettings::getAcceptCgi() const {
	return _acceptCgi;
}

int ConfigSettings::getAcceptMethod() const {
	return _acceptMethod;
}

bool ConfigSettings::getAutoIndex() const {
	return _autoIndex;
}

bool ConfigSettings::autoIndexIsSet() const {
	return _autoIndexIsSet;
}

void ConfigSettings::parseClientMaxBodySize(std::ifstream &infile) throw(ConfigParseException) {
	std::string token;
	char		lastChar;

	if (_clientMaxBodySize)
		throw  ConfigParseException("client_max_request_size already set");

	_clientMaxBodySize = 1;

	token = ParseUtils::parseValue(infile);
	lastChar = token[token.length() - 1];

	if (lastChar == 'K')
		_clientMaxBodySize *= 1024;
	if (lastChar == 'M')
		_clientMaxBodySize *= 1024 * 1024;
	if (lastChar == 'G') {
		_clientMaxBodySize *= 1024 * 1024 * 1024;
		token = token.substr(0, token.length() - 1);
	}
	_clientMaxBodySize *= ParseUtils::parseInt(token);

	ParseUtils::expectChar(infile, ';');
}

void ConfigSettings::parseErrorPage(std::ifstream &infile) throw(ConfigParseException) {
	std::string token;
	std::vector<int> errorCodes;

	token = ParseUtils::parseValue(infile);
	while (ParseUtils::isErrorCode(token)) {
		errorCodes.push_back(ParseUtils::parseInt(token));
		token = ParseUtils::parseValue(infile);
	}

	if (errorCodes.size() == 0)
		throw ConfigParseException("Invalid value for error page");
	if (!WebServUtils::fileExists(token))
		throw ConfigParseException("Error page file does not exist: " + token);

	for (unsigned int i = 0; i < errorCodes.size(); i++) {
		if (_errorPages.find(errorCodes[i]) != _errorPages.end())
			throw ConfigParseException("Multiple error pages for error code "
				+ WebServUtils::to_string(errorCodes[i]));
		_errorPages.insert(std::pair<int, std::string>(errorCodes[i], token));
	}

	ParseUtils::expectChar(infile, ';');
}

void ConfigSettings::parseIndex(std::ifstream &infile) throw(ConfigParseException) {
	std::string token;

	if (_index.size() > 0)
		throw ConfigParseException("index pages already set");

	token = ParseUtils::parseValue(infile);
	if (token == "")
		throw ConfigParseException("Invalid value for index");

	while (token != "") {
		_index.push_back(token);
		token = ParseUtils::parseValue(infile);
	}

	ParseUtils::expectChar(infile, ';');
}

void ConfigSettings::parseAcceptCgi(std::ifstream &infile) throw(ConfigParseException) {
	std::string token;

	if (_acceptCgi.size() > 0)
		throw ConfigParseException("accept_cgi already set");

	token = ParseUtils::parseValue(infile);
	if (token == "")
		throw ConfigParseException("Invalid value for accept cgi");

	while (token != "") {
		if (!WebServUtils::isin(CGI_EXTENSIONS, token))
			throw ConfigParseException("Invalid value for accept cgi: " + token);
		_acceptCgi.push_back(token);
		token = ParseUtils::parseValue(infile);
	}

	ParseUtils::expectChar(infile, ';');
}

void ConfigSettings::parseAcceptMethod(std::ifstream &infile) throw(ConfigParseException) {
	std::string	method;

	if (_acceptMethod != 0)
		throw  ConfigParseException("accept method already set");

	method = ParseUtils::parseValue(infile);
	while (method != "") {
		if (infile.fail())
			throw ConfigParseException("Error reading from file");
		if (infile.eof())
			throw ConfigParseException("Expected ';'");

		if (method == "GET")
			_acceptMethod |= METHOD_GET;
		else if (method == "POST")
			_acceptMethod |= METHOD_POST;
		else if (method == "DELETE")
			_acceptMethod |= METHOD_DELETE;
		else
			throw ConfigParseException("Invalid value for accept method: " + method);

		method = ParseUtils::parseValue(infile);
	}
	ParseUtils::expectChar(infile, ';');
}

void ConfigSettings::parseAutoIndex(std::ifstream &infile) throw(ConfigParseException) {
	std::string value = ParseUtils::parseValue(infile);

	if (_autoIndexIsSet)
		throw  ConfigParseException("autoindex value already set");

	if (value == "on")
		_autoIndex = true;
	else if (value == "off")
		_autoIndex = false;
	else
		throw ConfigParseException("Invalid value for auto index");

	ParseUtils::expectChar(infile, ';');
	_autoIndexIsSet = true;
}


