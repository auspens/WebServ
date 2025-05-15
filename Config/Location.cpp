/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wpepping <wpepping@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 12:40:08 by auspensk          #+#    #+#             */
/*   Updated: 2025/05/15 19:23:12 by wpepping         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Location.hpp"

Location::Location() :
		_autoindex(false),
		_uploadPass(false),
		_isRedirect(false),
		_path(""),
		_index("index.html") { }

Location::Location(const std::string &path) :
		_autoindex(false),
		_uploadPass(false),
		_isRedirect(false),
		_path(path),
		_index("index.html") { }

Location::~Location() { }

Location::Location(const Location &src) :
			_autoindex(src._autoindex),
			_uploadPass(src._uploadPass),
			_isRedirect(src._isRedirect),
			_path(src._path),
			_index(src._index) { }

Location &Location::operator =(const Location &other) {
	if (this != &other) {
		_autoindex = other._autoindex;
		_path = other._path;
		_index = other._index;
		_isRedirect = other._isRedirect;
	}
	return *this;
}

void Location::parse(std::ifstream &infile) throw(ConfigParseException) {
	std::string token;

	token = ParseUtils::parseValue(infile);
	_path = token;

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
		else
			throw ConfigParseException("Unexpected token: " + token);
		ParseUtils::skipWhitespace(infile);
	}
	ParseUtils::expectChar(infile, '}');
}

const std::string &Location::getPath() const {
	return _path;
}

const std::string &Location::getIndex() const {
	return _index;
}

const std::string &Location::getRootFolder() const {
	return _rootFolder;
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

int Location::getRedirectCode() const {
	return _redirect.code;
}

const std::string &Location::getRedirectPath() const {
	return _redirect.path;
}

const std::map<int, std::string> &Location::getErorrPages() const {
	return _configSettings.getErrorPages();
}

void Location::_parseRoot(std::ifstream &infile) throw(ConfigParseException) {
	std::string token;

	token = ParseUtils::parseValue(infile);
	ParseUtils::expectChar(infile, ';');

	if (!WebServUtils::folderExists(token))
		throw ConfigParseException("Root folder does not exist");

	_rootFolder = token;
}

void Location::_parseUploadPass(std::ifstream &infile) throw(ConfigParseException) {
	_uploadPass = true;
	_parseRoot(infile);
}

void Location::_parseRedirect(std::ifstream &infile) throw(ConfigParseException) {
	std::string token;

	token = ParseUtils::parseValue(infile);
	_redirect.code = ParseUtils::parseInt(token, 300, 308);
	_isRedirect = true;

	_redirect.path = ParseUtils::parseValue(infile);
	if (_redirect.path == "")
		throw ConfigParseException("Missing path for redirect");

	ParseUtils::expectChar(infile, ';');
}
