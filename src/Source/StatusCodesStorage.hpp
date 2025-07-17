/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   StatusCodesStorage.hpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wpepping <wpepping@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/12 18:24:25 by wouter            #+#    #+#             */
/*   Updated: 2025/07/17 15:25:46 by wpepping         ###   ########.fr       */
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
#include "Constants.hpp"

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
