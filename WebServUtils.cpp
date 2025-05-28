/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WebServUtils.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wouter <wouter@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 17:15:00 by wouter            #+#    #+#             */
/*   Updated: 2025/05/28 16:17:56 by wouter           ###   ########.fr       */
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
