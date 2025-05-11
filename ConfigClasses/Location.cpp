/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wouter <wouter@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 12:40:08 by auspensk          #+#    #+#             */
/*   Updated: 2025/05/11 20:59:09 by wouter           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Location.hpp"

Location::Location()
		: _path(""),
		_autoindex(false),
		_index("index.html"),
		_redirect(0, "") { }

Location::Location(const std::string &path)
		: _path(path),
		_autoindex(false),
		_index("index.html"),
		_redirect(0, "") { }

Location::~Location() { }

Location::Location(const Location &src)
			: _path(src._path),
			_autoindex(src._autoindex),
			_index(src._index),
			_redirect(src._redirect) { }

Location &Location::operator =(const Location &other) {
	if (this != &other) {
		_path = other._path;
		_autoindex = other._autoindex;
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

const std::map<int, std::string> &Location::getErorrPagesLocal() const {
	return _errorPagesLocal;
}

void Location::_parseRoot(std::ifstream &infile) throw(ConfigParseException) {
}
