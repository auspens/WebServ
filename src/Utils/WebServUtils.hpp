/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WebServUtils.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: auspensk <auspensk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 14:38:47 by wouter            #+#    #+#             */
/*   Updated: 2025/07/17 16:56:36 by auspensk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <cstdlib>
#include <dirent.h>
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <sys/epoll.h>
#include <vector>
#include "Logger.hpp"

class WebServUtils {
public:
	template <typename T>
	static std::string to_string(T src) {
		std::ostringstream result;
		result << src;
		return result.str();
	}

	static void			printErrorAndExit(std::string message, int exit_status);
	static bool			fileExists(const std::string &filename);
	static bool			folderExists(const std::string &dname);
	static bool			isin(const std::string haystack, char needle);
	static std::string	pathJoin(const std::string& path1, const std::string& path2);
	static bool			strEndsWith(const std::string& str, const std::string& end);
	static void			printVector(const std::vector<char>& vec);
	static std::string	num_to_str(size_t num);
	static std::string	getEpollEventNames(uint32_t events);
	static void			removeFromString(std::string &haystack, const std::string &needle);
	static std::string	getCgiExtension(const std::string &path, std::map<std::string, std::string> acceptCgi);
	static std::string	urlDecode(const std::string &encoded);
	static std::string	trim(const std::string& s);
	
	template <typename T, size_t N>
	static bool isin(const T (&array)[N], const T &value) 	{
		for (size_t i = 0; i < N; i++) {
			if (array[i] == value)
				return true;
		}
		return false;
	}

template <typename Container>
static void removeFromContainer(Container& container, const typename Container::value_type &value) {
	typename Container::iterator it = std::find(container.begin(), container.end(), value);
	if (it != container.end()) {
		container.erase(it);
	}
	}

private:
	WebServUtils();
	~WebServUtils();

	static bool _isHex(char c);
};
