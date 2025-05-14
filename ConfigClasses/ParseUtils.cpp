/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ParseUtils.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wouter <wouter@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/09 16:03:09 by wpepping          #+#    #+#             */
/*   Updated: 2025/05/14 17:33:02 by wouter           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ParseUtils.hpp"

ParseUtils::ParseUtils() { }

ParseUtils::~ParseUtils() { }

void ParseUtils::expectWhitespace(std::ifstream &infile) throw(ConfigParseException) {
	char c;

	c = infile.peek();
	if (infile.eof() || !std::isspace(c))
		throw ConfigParseException("Expected space or tab");
	if (infile.fail())
		throw ConfigParseException("Error reading from file");
	skipWhitespace(infile);
}

void ParseUtils::skipWhitespace(std::ifstream &infile) throw(ConfigParseException) {
	char c;

	c = infile.peek();
	while (!infile.eof() && std::isspace(c)) {
		infile.get();
		if (infile.fail())
			throw ConfigParseException("Error reading from file");
		c = infile.peek();
	}
}

bool ParseUtils::expectChar(std::ifstream &infile, char c) throw(ConfigParseException) {
	char d;

	infile >> d;
	if (infile.fail())
		throw ConfigParseException("Error reading from file");
	if (infile.eof() || infile.fail() || d != c)
		return false;
	return true;
}

std::string ParseUtils::parseToken(std::ifstream &infile) throw(ConfigParseException) {
	std::string result;

	infile >> result;
	if (infile.fail() && !infile.eof())
		throw ConfigParseException("Error reading from file");

	return result;
}

int ParseUtils::parseInt(std::string nbr) throw(ConfigParseException) {
	return parseInt(nbr, std::numeric_limits<int>::min(),
		std::numeric_limits<int>::max());
}

int ParseUtils::parseInt(std::string nbr, int min, int max) throw(ConfigParseException) {
	const char				*c_str;
	char					*str_end;
	long					l;

	c_str = nbr.c_str();
	l = strtol(c_str, &str_end, 10);
	if (c_str == str_end || l < std::numeric_limits<int>::min()
		|| l > std::numeric_limits<int>::max())
			throw ConfigParseException("Invalid number");
}

std::string ParseUtils::parseValue(std::ifstream &infile) throw(ConfigParseException) {
	char c;
	std::string result = "";

	skipWhitespace(infile);
	c = infile.peek();
	while (!infile.fail() && !infile.eof() && !std::isspace(c) && c != ';')
		result += infile.get();

	if (infile.fail() && !infile.eof())
		throw ConfigParseException("Error reading from file");

	return result;
}
