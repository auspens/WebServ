/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PageGenerator.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wouter <wouter@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/12 17:50:33 by wouter            #+#    #+#             */
/*   Updated: 2025/07/12 18:30:39 by wouter           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <dirent.h>
#include <string>
#include <sys/stat.h>
#include <vector>
#include "Config.hpp"
#include "Constants.hpp"
#include "SourceAndRequestException.hpp"
#include "StatusCodesStorage.hpp"

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

class PageGenerator {
	public:
		static std::string generateIndex(
			std::string relativeTarget,
			std::string folder,
			bool acceptsDelete
		);
		static std::string generateErrorPage(int code);

	private:
		static bool readDirectories(std::vector<DirEntry> &entries, std::string folder);
};
