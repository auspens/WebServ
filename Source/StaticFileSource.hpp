/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   StaticFileSource.hpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wouter <wouter@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 10:08:43 by auspensk          #+#    #+#             */
/*   Updated: 2025/07/06 17:51:05 by wouter           ###   ########.fr       */
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

#define JAVASCRIPT_DELETE_FUNCTION \
"<script>\n" \
"\n" \
"async function deleteFile(filename) {\n" \
"	if (confirm('Are you sure you want to delete ' + filename + '?')) {\n" \
"		try {\n" \
"			const response = await fetch(filename, {\n" \
"				method: 'DELETE'\n" \
"			});\n" \
"\n" \
"			if (response.ok) {\n" \
"				alert('File deleted successfully');\n" \
"				location.reload();\n" \
"			} else {\n" \
"				alert('Failed to delete file: ' + response.status);\n" \
"			}\n" \
"		} catch (error) {\n" \
"			alert('Error: ' + error.message);\n" \
"		}\n" \
"	}\n" \
"}\n" \
"</script>\n"

struct DirEntry {
	std::string	name;
	bool		is_directory;

	DirEntry(const std::string& n, bool is_dir) : name(n), is_directory(is_dir) {}
};

class StaticFileSource : public Source {
	public:
		void readSource() throw(SourceAndRequestException);
		char *readFromBuffer();

		StaticFileSource(const ServerConfig &serverConfig, Location const *location, HttpRequest &req);
		StaticFileSource(const ServerConfig &serverConfig, Location const *location, HttpRequest &req, int code)throw();
		StaticFileSource(const StaticFileSource &src);
		StaticFileSource &operator=(StaticFileSource const &other);
		~StaticFileSource();

	protected:
		bool _generated;
		void checkIfDirectory() throw(SourceAndRequestException);
		void defineMimeType();
		void generateIndex() throw(SourceAndRequestException);
		bool readDirectories(std::vector<DirEntry>&entries);
		void generatePage(int code);
		bool indexExists(const std::vector<std::string> &indexes, const std::string &root);
};
