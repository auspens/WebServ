/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   StaticFileSource.hpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wpepping <wpepping@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 10:08:43 by auspensk          #+#    #+#             */
/*   Updated: 2025/06/19 17:45:24 by wpepping         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Config.hpp"
#include "Logger.hpp"
#include "Source.hpp"
#include "SystemCallsUtilities.hpp"
#include "WebServUtils.hpp"
#include "ServerConfig.hpp"
#include <dirent.h>

struct DirEntry {
    std::string name;
    bool is_directory;
	DirEntry(const std::string& n, bool is_dir) : name(n), is_directory(is_dir) {}
};

class StaticFileSource : public Source {
	public:
		void readSource();
		char *readFromBuffer();

		StaticFileSource(const ServerConfig &serverConfig, Location const *location, HttpRequest req);
		StaticFileSource(const ServerConfig &serverConfig, Location const *location, HttpRequest req, int code)throw();
		StaticFileSource(const StaticFileSource &src);
		StaticFileSource &operator=(StaticFileSource const &other);
		~StaticFileSource();

	protected:
		bool _generated;
		bool checkIfExists(std::string &target);
		void checkIfDirectory();
		void defineMimeType();
		bool generateIndex();
		bool readDirectories(std::vector<DirEntry>&entries);
		void generatePage(int code);
		bool indexExists(const std::vector<std::string> &indexes, const std::string &root);
};
