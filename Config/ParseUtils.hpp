/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ParseUtils.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wpepping <wpepping@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/09 16:03:05 by wpepping          #+#    #+#             */
/*   Updated: 2025/05/23 16:58:08 by wpepping         ###   ########.fr       */
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

	template <typename T>
	static T coalesce(T v1, T v2) {
		if (v1) return v1;
		return v2;
	}

private:
	ParseUtils();
	~ParseUtils();
};
