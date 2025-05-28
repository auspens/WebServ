/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   StaticFileSource.hpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wouter <wouter@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 10:08:43 by auspensk          #+#    #+#             */
/*   Updated: 2025/05/28 16:07:55 by wouter           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
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
		char *getBufferToSend();//returns a buffer that can be sent through socket

		StaticFileSource(const ServerConfig &serverConfig, Location const *location, HttpRequest req);
		~StaticFileSource();

	private:
		bool _generated;
		bool checkIfExists(std::string &target);
		void checkIfDirectory();
		void defineMimeType();
		bool generateIndex();
		void getErrorPage(int index);
		bool readDirectories(std::vector<DirEntry>&entries);
		void generatePage(int code);
		bool indexExists(const std::vector<std::string> &indexes, const std::string &root);
};
