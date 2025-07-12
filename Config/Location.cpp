/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wouter <wouter@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 12:40:08 by auspensk          #+#    #+#             */
/*   Updated: 2025/07/12 17:20:04 by wouter           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Location.hpp"
#include "ServerConfig.hpp"

Location::Location() :
	_autoindex(DEFAULT_AUTO_INDEX),
	_uploadPass(false),
	_isRedirect(false),
	_isShutDown(false),
	_path(""),
	_index(DEFAULT_INDEX),
	_serverConfig(NULL) { }

Location::Location(const ServerConfig &serverConfig) :
	_autoindex(DEFAULT_AUTO_INDEX),
	_uploadPass(false),
	_isRedirect(false),
	_isShutDown(false),
	_path(""),
	_index(DEFAULT_INDEX),
	_serverConfig(&serverConfig) { }

Location::~Location() { }

Location::Location(const Location &src) :
	_autoindex(src._autoindex),
	_uploadPass(src._uploadPass),
	_isRedirect(src._isRedirect),
	_isShutDown(src._isShutDown),
	_path(src._path),
	_index(src._index),
	_serverConfig(src._serverConfig) { }

Location &Location::operator =(const Location &other) {
	if (this != &other) {
		_autoindex = other._autoindex;
		_uploadPass = other._uploadPass;
		_isRedirect = other._isRedirect;
		_isShutDown = other._isShutDown;
		_path = other._path;
		_index = other._index;
		_serverConfig = other._serverConfig;
	}
	return *this;
}

void Location::parse(std::ifstream &infile) throw(ConfigParseException) {
	std::string token;

	ParseUtils::expectChar(infile, '/');

	token = ParseUtils::parseValue(infile);
	_path = '/' + token;

	if (_path == "")
		throw ConfigParseException("Missing path for location");

	ParseUtils::expectChar(infile, '{');
	while (infile.peek() != '}' && !infile.eof()) {
		token = ParseUtils::parseToken(infile);
		ParseUtils::expectWhitespace(infile);
		if (_configSettings.isConfigSetting(token))
			_configSettings.parseConfigSetting(infile, token);
		else if (token == "root")
			_parseRoot(infile);
		else if (token == "upload_pass")
			_parseUploadPass(infile);
		else if (token == "redirect")
			_parseRedirect(infile);
		else if (token == "shutdown")
			_parseShutDown(infile);
		else
			throw ConfigParseException("Unexpected token: " + token);
		ParseUtils::skipWhitespace(infile);
	}
	ParseUtils::expectChar(infile, '}');
}

const std::string &Location::getPath() const {
	return _path;
}

const std::string &Location::getRootFolder() const {
	if (_rootFolder != "")
		return _rootFolder;
	return _serverConfig->getRootFolder();
}

bool Location::autoindexOn() const {
	return _autoindex;
}

bool Location::isUploadPass() const {
	return _uploadPass;
}

bool Location::isRedirect() const {
	return _isRedirect;
}

bool Location::isShutDown() const {
	return _isShutDown;
}

int Location::getRedirectCode() const {
	return _redirect.code;
}

const std::string &Location::getRedirectPath() const {
	return _redirect.path;
}

size_t Location::getClientMaxBodySize() const {
	if (_configSettings.getClientMaxBodySize())
		return _configSettings.getClientMaxBodySize();
	if (_serverConfig)
		return _serverConfig->getClientMaxBodySize();
	return DEFAULT_client_max_request_size;
}

const std::map<int, std::string>& Location::getErrorPages() const {
	if (!_configSettings.getErrorPages().empty())
		return _configSettings.getErrorPages();
	if (_serverConfig)
		return _serverConfig->getErrorPages();
	return _configSettings.getErrorPages();
}

const std::vector<std::string>& Location::getIndexPages() const {
	if (!_configSettings.getIndexPages().empty())
		return _configSettings.getIndexPages();
	if (_serverConfig)
		return _serverConfig->getIndexPages();
	return _configSettings.getIndexPages();
}

const std::vector<std::string>& Location::getAcceptCgi() const {
	if (!_configSettings.getAcceptCgi().empty())
		return _configSettings.getAcceptCgi();
	if (_serverConfig)
		return _serverConfig->getAcceptCgi();
	return _configSettings.getAcceptCgi();
}

int Location::getAcceptMethod() const {
	if (_configSettings.getAcceptMethod())
		return _configSettings.getAcceptMethod();
	if (_serverConfig)
		return _serverConfig->getAcceptMethod();
	return DEFAULT_ACCEPT_METHOD;
}

bool Location::getAutoIndex() const {
	if (_configSettings.autoIndexIsSet())
		return _configSettings.getAutoIndex();
	if (_serverConfig)
		return _serverConfig->getAutoIndex();
	return DEFAULT_AUTO_INDEX;
}

void Location::_parseRoot(std::ifstream &infile) throw(ConfigParseException) {
	std::string token;

	token = ParseUtils::parseValue(infile);
	ParseUtils::expectChar(infile, ';');

	if (_rootFolder != "")
		throw ConfigParseException("Location: " + _path + ": Root folder already set");
	if (_isRedirect)
		throw ConfigParseException("Location: " + _path + ": Cannot have root folder for redirect");
	if (_isShutDown)
		throw ConfigParseException("Location: " + _path + ": Cannot have root folder for shutdown");
	if (!WebServUtils::folderExists(token))
		throw ConfigParseException("Location: " + _path + ": Root folder " + token + " does not exist");

	_rootFolder = token;
}

void Location::_parseUploadPass(std::ifstream &infile) throw(ConfigParseException) {
	_uploadPass = true;
	_parseRoot(infile);
}

void Location::_parseShutDown(std::ifstream &infile) throw(ConfigParseException) {
	std::string value = ParseUtils::parseValue(infile);

	if (_isShutDown)
		throw  ConfigParseException("shutdown value already set");
	if (_isRedirect)
		throw ConfigParseException("Location: " + _path + ": Shutdown can't be redirect");
	if (_rootFolder != "")
		throw ConfigParseException("Location: " + _path + ": Cannot have root folder for shutdown");

	if (value == "yes")
		_isShutDown = true;
	else
		throw ConfigParseException("Invalid value for shutdown");

	ParseUtils::expectChar(infile, ';');
}

void Location::_parseRedirect(std::ifstream &infile) throw(ConfigParseException) {
	std::string token;

	token = ParseUtils::parseValue(infile);
	_redirect.code = ParseUtils::parseLong(token, 300, 308);
	_isRedirect = true;

	_redirect.path = ParseUtils::parseValue(infile);
	if (_rootFolder != "")
		throw ConfigParseException("Location: " + _path + ": Cannot have root folder for redirect");
	if (_isShutDown)
		throw ConfigParseException("Location: " + _path + ": Shutdown can't be redirect");
	if (_redirect.path == "")
		throw ConfigParseException("Location: " + _path + ": Missing path for redirect");

	ParseUtils::expectChar(infile, ';');
}
