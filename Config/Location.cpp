/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wpepping <wpepping@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 12:40:08 by auspensk          #+#    #+#             */
/*   Updated: 2025/05/15 15:00:54 by wpepping         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Location.hpp"

Location::Location() :
		_autoindex(false),
		_path(""),
		_index("index.html"),
		_redirect(0, "") { }

Location::Location(const std::string &path) :
		_autoindex(false),
		_path(path),
		_index("index.html"),
		_redirect(0, "") { }

Location::~Location() { }

Location::Location(const Location &src) :
			_autoindex(src._autoindex),
			_path(src._path),
			_index(src._index),
			_redirect(src._redirect) { }

Location &Location::operator =(const Location &other) {
	if (this != &other) {
		_autoindex = other._autoindex;
		_path = other._path;
		_index = other._index;
		_redirect = other._redirect;
	}
	return *this;
}

void Location::parse(std::ifstream &infile) throw(ConfigParseException) {
	std::string token;

	while (infile.peek() != '}' && !infile.eof()) {
		token = ParseUtils::parseToken(infile);
		ParseUtils::expectWhitespace(infile);
		if (_configSettings.isConfigSetting(token))
			_configSettings.parseConfigSetting(infile, token);
		else if (token == "root")
			_parseRoot(infile);
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

bool Location::autoindexOn() const {
	return _autoindex;
}

const std::pair<int, std::string> &Location::getRedirect() const {
	return _redirect;
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
