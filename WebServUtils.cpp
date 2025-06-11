/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WebServUtils.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: auspensk <auspensk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 17:15:00 by wouter            #+#    #+#             */
/*   Updated: 2025/06/11 13:31:03 by auspensk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "WebServUtils.hpp"

void WebServUtils::printErrorAndExit(std::string message, int exit_status) {
	std::cout << message << std::endl;
	std::exit(exit_status);
}

bool WebServUtils::fileExists(const std::string &filename) {
	std::ifstream file(filename.c_str());
	return file.is_open();
}

bool	WebServUtils::folderExists(const std::string &dname) {
	DIR	*dir;

	dir = opendir(dname.c_str());
	if (!dir)
		return (false);
	closedir(dir);
	return (true);
}

bool WebServUtils::isin(const std::string haystack, char needle) {
	return (haystack.find(needle) != std::string::npos);
}

bool WebServUtils::strEndsWith(const std::string& str, const std::string& end) {
	if (end.size() > str.size())
		return false;
	return str.compare(str.size() - end.size(), end.size(), end) == 0;
}

std::string WebServUtils::pathJoin(const std::string& path1, const std::string& path2) {
	int slashes = 0;

	if (path1.empty())
		return path2;
	if (path2.empty())
		return path1;

	if (path1[path1.size() - 1] == '/')
		slashes++;
	if (path2[0] == '/')
		slashes++;

	if (slashes == 0)
		return path1 + "/" + path2;
	if (slashes == 1)
		return path1 + path2;
	return path1 + path2.substr(1);
}


void WebServUtils::printVector(const std::vector<char>& vec) {
    for (std::vector<char>::const_iterator it = vec.begin(); it != vec.end(); ++it) {
        std::cout << *it;
    }
    std::cout << std::endl;
}
