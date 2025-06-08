/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WebServUtils.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wouter <wouter@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 14:38:47 by wouter            #+#    #+#             */
/*   Updated: 2025/06/08 13:49:33 by wouter           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <dirent.h>

class WebServUtils {
public:
	template <typename T>
	static std::string to_string(T src) {
		std::ostringstream result;
		result << src;
		return result.str();
	}

	void printErrorAndExit(std::string message, int exit_status);
	static bool fileExists(const std::string &filename);
	static bool folderExists(const std::string &dname);
	static bool isin(const std::string haystack, char needle);
	static std::string pathJoin(const std::string& path1, const std::string& path2);
	static bool strEndsWith(const std::string& str, const std::string& end);


	template <typename T, size_t N>
	static bool isin(const T (&array)[N], const T &value) 	{
		for (size_t i = 0; i < N; i++) {
			if (array[i] == value)
				return true;
		}
		return false;
	}

private:
	WebServUtils();
	~WebServUtils();
};
