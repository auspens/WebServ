/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ParseUtils.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wpepping <wpepping@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/09 16:03:05 by wpepping          #+#    #+#             */
/*   Updated: 2025/05/09 18:17:34 by wpepping         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <fstream>
#include <limits>
#include <string>
#include "ConfigParseException.hpp"

class ParseUtils {
public:
	static bool expectChar(std::ifstream &infile, char c);
	static std::string parseToken(std::ifstream &infile);
	static void expectWhitespace(std::ifstream &infile) throw(ConfigParseException);
	static void skipWhitespace(std::ifstream &infile);
	static int parseInt(std::string nbr, int min, int max);
	static int parseInt(std::string nbr);
};
