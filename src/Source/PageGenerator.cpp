#include "PageGenerator.hpp"

std::string PageGenerator::generateIndex(
	std::string relativeTarget,
	std::string folder,
	bool acceptsDelete
) {
	std::vector<DirEntry> entries;
	if (!readDirectories(entries, folder))
		throw SourceAndRequestException("Could not generate index", 500);

	std::string html =
		DOCSTRING
		"<html><head><title>Index of " + relativeTarget + "</title></head>\n"
		"<body><h1> Index of " + relativeTarget + "</h1>"
		"<hr><table>\n";

	for (size_t i = 0; i < entries.size(); ++i){
		std::string name = entries[i].name;
		std::string href = relativeTarget + "/" + name;
		if (entries[i].is_directory) {
			href += "/";
			name += "/";
		}
		html += "<tr><td style=\"padding-left: 20px;\"><a href=\"" + href + "\">" + name + "</a></td>\n";

		if (acceptsDelete)
			html += "<td style=\"padding-left: 50px;\"><a href=\"#\" onclick=\"deleteFile('" + href + "'); return false;\">delete</a></td></tr>\n";
	}

	html += "</table><hr>";

	if (acceptsDelete)
		html += JAVASCRIPT_DELETE_FUNCTION;

	html +=	"</body></html>\n";

	return html;
}

std::string PageGenerator::generateErrorPage(int code) {
	static const std::map<int, HTTPStatusCode> statusCodes = StatusCodesStorage::getStatusCodes();

	std::string html =
		DOCSTRING
		"<html><head><title> " + statusCodes.at(code).message + "</title></head>"
		"<body><h1>" + statusCodes.at(code).code + " " + statusCodes.at(code).message + "</h1>"
		"<div>" + statusCodes.at(code).description + "</div>"
		"</body></html>";

	return html;
}

bool PageGenerator::readDirectories(std::vector<DirEntry> &entries, std::string folder) {
	Logger::debug() << "Target: " << folder << std::endl;
	DIR *dir = opendir(folder.c_str());
	if (!dir)
		return false;
	struct dirent *entry;
	while ((entry = readdir(dir)) != NULL){
		std::string name = entry->d_name;
		if (name == "." || name == "..")
			continue;

		std::string full_path = folder + "/" + name;
		struct stat s;
		if (stat(full_path.c_str(), &s) == 0){
			bool is_dir = S_ISDIR(s.st_mode);
			entries.push_back(DirEntry(name, is_dir));
		}
	}
	closedir(dir);
	return true;
}
