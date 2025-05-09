/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ParseUtils.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wpepping <wpepping@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/09 16:03:09 by wpepping          #+#    #+#             */
/*   Updated: 2025/05/09 19:08:22 by wpepping         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ParseUtils.hpp"

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
	while (std::isspace(c)) {
		infile.get();
		c = infile.peek();
	}
}

bool ParseUtils::expectChar(std::ifstream &infile, char c) {
	char d;

	infile >> d;
	if (infile.eof() || infile.fail() || d != c)
		return false;
	return true;
}

std::string ParseUtils::parseToken(std::ifstream &infile) {
	std::string result;

	infile >> result;
	return result;
}

int ParseUtils::parseInt(std::string nbr) {
	return parseInt(nbr, std::numeric_limits<int>::min(),
		std::numeric_limits<int>::max());
}

int ParseUtils::parseInt(std::string nbr, int min, int max) {
	const char				*c_str;
	char					*str_end;
	long					l;

	c_str = nbr.c_str();
	l = strtol(c_str, &str_end, 10);
	if (c_str == str_end || l < std::numeric_limits<int>::min()
		|| l > std::numeric_limits<int>::max())
			throw ConfigParseException("Invalid number");
}
