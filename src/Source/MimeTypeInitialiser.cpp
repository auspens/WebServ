/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MimeTypeInitialiser.cpp                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wpepping <wpepping@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 15:48:25 by auspensk          #+#    #+#             */
/*   Updated: 2025/07/17 15:27:32 by wpepping         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <map>
#include "Constants.hpp"
#include "Source.hpp"

std::map <std::string, std::string> Source::_mimeTypes;

std::string trim(const std::string& str) {
	size_t start = 0, end = str.length();
	while (start < end && std::isspace(str[start])) start++;
	while (end > start && std::isspace(str[end - 1])) end--;
	if (start < end && str[start] == '\"') ++start;
	if (end > start && str[end - 1] == '\"') --end;
	return str.substr(start, end - start);
}

struct mimeTypesInitializer {
	mimeTypesInitializer(){
		std::ifstream file(MIME_TYPES_JSON);
		if (!file)
			throw (std::runtime_error("Could not open json with mime types"));
		std::string line;

		while (std::getline(file, line)) {
			std::string::size_type colon = line.find(':');
			if (colon == std::string::npos) continue;
			std::string key = trim(line.substr(0, colon));
			std::string val = line.substr(colon + 1);
			if (!val.empty() && val[val.size() - 1] == ',')
							val = val.substr(0, val.size() - 1);
			val = trim(val);
			Source::_mimeTypes[key] = val;
		}
	}
};

static mimeTypesInitializer _mimeTypeInitializer;
