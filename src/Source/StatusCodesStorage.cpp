/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   StatusCodesStorage.cpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wouter <wouter@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/12 18:25:01 by wouter            #+#    #+#             */
/*   Updated: 2025/07/12 19:21:04 by wouter           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "StatusCodesStorage.hpp"

std::map<int, HTTPStatusCode> StatusCodesStorage::_statusCodes;

const std::map<int, HTTPStatusCode>& StatusCodesStorage::getStatusCodes() {
	if (_statusCodes.empty()) {
		loadStatusCodes();
	}
	return _statusCodes;
}

void StatusCodesStorage::loadStatusCodes() {
	std::ifstream file(STATUS_CODES_JSON);
	if (!file.is_open()) {
		std::cerr << "Cannot open status codes json\n";
		return;
	}

	std::string line;
	int currentCode = -1;
	HTTPStatusCode current;

	while (std::getline(file, line)) {
		line = trim(line);

		if (line.empty()) continue;

		if (line[0] == '"') {
			size_t endQuote = line.find('"', 1);
			if (endQuote == std::string::npos) continue;

			std::string key = line.substr(1, endQuote - 1);
			if (isDigitString(key)) {
				currentCode = std::atoi(key.c_str());
				current = HTTPStatusCode();
				continue;
			}
		}

		if (currentCode == -1) continue;

		if (line.find("\"code\"") != std::string::npos) {
			current.code = extractCode(line);
		} else if (line.find("\"message\"") != std::string::npos) {
			current.message = extractValue(line);
		} else if (line.find("\"description\"") != std::string::npos) {
			current.description = extractValue(line);
		} else if (line.find('}') != std::string::npos) {
			_statusCodes[std::atoi(current.code.c_str())] = current;
			currentCode = -1;
		}
	}

	file.close();
}

bool StatusCodesStorage::isDigitString(const std::string& s) {
	for (size_t i = 0; i < s.size(); ++i)
		if (!std::isdigit(s[i])) return false;
	return true;
}

std::string StatusCodesStorage::trim(const std::string& s) {
	size_t start = s.find_first_not_of(" \t\n\r");
	size_t end = s.find_last_not_of(" \t\n\r");
	if (start == std::string::npos || end == std::string::npos)
		return "";
	return s.substr(start, end - start + 1);
}

std::string StatusCodesStorage::extractValue(const std::string& line) {
	size_t colon = line.find(':');
	if (colon == std::string::npos) return "";

	size_t firstQuote = line.find('"', colon);
	size_t lastQuote = line.find_last_of('"');
	if (firstQuote == std::string::npos || lastQuote == std::string::npos || firstQuote == lastQuote)
		return "";

	return line.substr(firstQuote + 1, lastQuote - firstQuote - 1);
}

std::string StatusCodesStorage::extractCode(const std::string& line) {
	size_t colon = line.find(':');
	if (colon == std::string::npos) return "";

	size_t firstSpace = line.find(' ', colon);
	size_t lastComma = line.find_last_of(',');
	if (firstSpace == std::string::npos || lastComma == std::string::npos || firstSpace == lastComma)
		return "";

	return line.substr(firstSpace + 1, lastComma - firstSpace - 1);
}
