/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   StatusCodesInitialiser.cpp                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: auspensk <auspensk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 15:57:09 by auspensk          #+#    #+#             */
/*   Updated: 2025/05/26 16:49:44 by auspensk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Source.hpp"
#include "RedirectSource.hpp"
#include "StaticFileSource.hpp"
#include "CGISource.hpp"

std::map<int, HTTPStatusCode> Source::_statusCodes;

#include <iostream>
#include <fstream>
#include <map>
#include <string>
#include <sstream>
#include <cctype>

bool is_digit_string(const std::string& s) {
	for (size_t i = 0; i < s.size(); ++i)
		if (!std::isdigit(s[i])) return false;
	return true;
}

std::string trim2(const std::string& s) {
	size_t start = s.find_first_not_of(" \t\n\r");
	size_t end = s.find_last_not_of(" \t\n\r");
	if (start == std::string::npos || end == std::string::npos)
		return "";
	return s.substr(start, end - start + 1);
}

std::string extract_value(const std::string& line) {
	size_t colon = line.find(':');
	if (colon == std::string::npos) return "";

	size_t first_quote = line.find('"', colon);
	size_t last_quote = line.find_last_of('"');
	if (first_quote == std::string::npos || last_quote == std::string::npos || first_quote == last_quote)
		return "";

	return line.substr(first_quote + 1, last_quote - first_quote - 1);
}

struct statusCodesInitialiser{
	statusCodesInitialiser() {
		load_http_status_codes();
	}
	void load_http_status_codes() {
	std::ifstream file(STATUS_CODES_JSON);
	if (!file.is_open()) {
		std::cerr << "Cannot open status codes json\n";
		return;
	}

	std::string line;
	int current_code = -1;
	HTTPStatusCode current;

	while (std::getline(file, line)) {
		line = trim2(line);

		// Check for start of a new entry like: "100": {
		if (line[0] == '"') {
			size_t end_quote = line.find('"', 1);
			if (end_quote == std::string::npos) continue;

			std::string key = line.substr(1, end_quote - 1);
			if (is_digit_string(key)) {
				current_code = std::atoi(key.c_str());
				current = HTTPStatusCode(); // reset
			} else {
				current_code = -1; // skip non-numeric
			}
		}

		if (current_code == -1)
			continue;

		if (line.find("\"code\"") != std::string::npos) {
			current.code = extract_value(line);
		} else if (line.find("\"message\"") != std::string::npos) {
			current.message = extract_value(line);
		} else if (line.find("\"description\"") != std::string::npos) {
			current.description = extract_value(line);
		} else if (line.find('}') != std::string::npos) {
			Source::_statusCodes[std::atoi(current.code.c_str())] = current;
			current_code = -1;
		}
	}
	file.close();
}
};
static statusCodesInitialiser _statusCodeInitializer;
