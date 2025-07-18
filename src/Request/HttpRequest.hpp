#pragma once
#include <string>
#include <map>

struct HttpRequest {
	std::string method;
	std::string uri;
	std::string hostname;
	std::string path;
	std::string http_version;
	std::map<std::string, std::string> headers;
	std::string body;

	bool isNotKeepAlive() {
		std::map<std::string, std::string>::iterator it = headers.find("Connection");
		return (it != headers.end() && it->second == "close");
	}
};
