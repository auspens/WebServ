/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ParseUtils.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wpepping <wpepping@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/09 16:03:05 by wpepping          #+#    #+#             */
/*   Updated: 2025/05/15 18:54:56 by wpepping         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <fstream>
#include <limits>
#include <cstdlib>
#include <string>
#include "ConfigParseException.hpp"

class ParseUtils {
public:
	static bool expectChar(std::ifstream &infile, char c) throw(ConfigParseException);
	static std::string parseToken(std::ifstream &infile) throw(ConfigParseException);
	static void expectWhitespace(std::ifstream &infile) throw(ConfigParseException);
	static void skipWhitespace(std::ifstream &infile) throw(ConfigParseException);
	static int parseInt(std::string nbr, int min, int max) throw(ConfigParseException);
	static int parseInt(std::string nbr) throw(ConfigParseException);
	static std::string parseValue(std::ifstream &infile) throw(ConfigParseException);
	static bool isErrorCode(std::string const &str);

private:
	ParseUtils();
	~ParseUtils();
};
