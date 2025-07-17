#include "WebServUtils.hpp"

void WebServUtils::printErrorAndExit(std::string message, int exit_status) {
	Logger::error() << message << std::endl;
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

std::string WebServUtils::num_to_str(size_t num) {
	std::ostringstream convert;
	convert << num;
	return convert.str();
}

std::string WebServUtils::getEpollEventNames(uint32_t events) {
	std::vector<std::string> names;

	if (events & EPOLLIN) names.push_back("EPOLLIN");
	if (events & EPOLLOUT) names.push_back("EPOLLOUT");
	if (events & EPOLLERR) names.push_back("EPOLLERR");
	if (events & EPOLLHUP) names.push_back("EPOLLHUP");
	if (events & EPOLLRDHUP) names.push_back("EPOLLRDHUP");
	if (events & EPOLLET) names.push_back("EPOLLET");
	if (events & EPOLLONESHOT) names.push_back("EPOLLONESHOT");

	std::string result;
	for (size_t i = 0; i < names.size(); ++i) {
		result += names[i];
		if (i != names.size() - 1) {
			result += ", ";
		}
	}

	return result;
}

void WebServUtils::removeFromString(std::string &haystack, const std::string &needle) {
	size_t pos;
	size_t len = needle.length();

	while ((pos = haystack.find(needle)) != std::string::npos)
		haystack.erase(pos, len);
}

std::string WebServUtils::getCgiExtension(const std::string &path, std::map<std::string, std::string> acceptCgi) {
	std::map<std::string, std::string>::const_iterator it;
	for ( it = acceptCgi.begin(); it != acceptCgi.end(); ++it) {
		if (WebServUtils::strEndsWith(path, it->first))
			return it->first;
	}
	return "";
}

std::string WebServUtils::urlDecode(const std::string &encoded) {
	std::string	result = "";

	for (size_t i = 0; i < encoded.length(); i++) {
		if (encoded[i] == '%' && i + 2 < encoded.length()
			&& std::isxdigit(encoded[i + 1]) && std::isxdigit(encoded[i + 2])) {
				result += static_cast<char>(16 * (encoded[i + 1] - '0') + (encoded[i + 2] - '0'));
				i += 2;
		} else if (encoded[i] == '+') {
			result += ' ';
		} else
			result += encoded[i];
	}
	return result;
}

std::string WebServUtils::trim(const std::string& s) {
	size_t start = s.find_first_not_of(" \t");
	if (start == std::string::npos)
		return "";
	size_t end = s.find_last_not_of(" \t");
	return s.substr(start, end - start + 1);
}

WebServUtils::WebServUtils() { }
WebServUtils::~WebServUtils() { }
