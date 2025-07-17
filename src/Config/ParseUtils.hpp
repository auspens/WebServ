/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ParseUtils.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wpepping <wpepping@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/09 16:03:05 by wpepping          #+#    #+#             */
/*   Updated: 2025/06/18 15:57:38 by wpepping         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <fstream>
#include <limits>
#include <cstdlib>
#include <string>
#include "ConfigParseException.hpp"
#include "WebServUtils.hpp"

class ParseUtils {
public:
	static void expectChar(std::ifstream &infile, char c) throw(ConfigParseException);
	static std::string parseToken(std::ifstream &infile) throw(ConfigParseException);
	static void expectWhitespace(std::ifstream &infile) throw(ConfigParseException);
	static void skipWhitespace(std::ifstream &infile) throw(ConfigParseException);
	static long parseLong(std::string nbr, long min, long max) throw(ConfigParseException);
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
