/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   StatusCodesStorage.hpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wouter <wouter@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/12 18:24:25 by wouter            #+#    #+#             */
/*   Updated: 2025/07/12 19:21:25 by wouter           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <cctype>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <map>
#include <sstream>
#include <string>

#define STATUS_CODES_JSON "StatusCodes.json"
#define MIME_TYPES_JSON "FileExtesionsToMime.json"

struct HTTPStatusCode {
	std::string code;
	std::string message;
	std::string description;
};

class StatusCodesStorage {
	public:
		static const std::map<int, HTTPStatusCode> &getStatusCodes();

	private:
		static std::map<int, HTTPStatusCode> _statusCodes;
		static void loadStatusCodes();

		static bool isDigitString(const std::string& s);
		static std::string trim(const std::string& s);
		static std::string extractCode(const std::string& line);
		static std::string extractValue(const std::string& line);
};
