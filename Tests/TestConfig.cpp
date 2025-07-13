#include "TestConfig.hpp"

static std::string join(const std::vector<std::string>& vec, const std::string& sep = " ") {
	std::ostringstream oss;
	for (size_t i = 0; i < vec.size(); ++i) {
		if (i > 0) oss << sep;
		oss << vec[i];
	}
	return oss.str();
}

static std::string join(const std::map<std::string, std::string>& map, const std::string& sep = ", ") {
	std::ostringstream oss;
	for (std::map<std::string, std::string>::const_iterator it = map.begin(); it != map.end(); ++it) {
		if (it != map.begin()) {
			oss << sep;
		}
		oss << it->first << ": " << it->second;
	}
	return oss.str();
}

void printFullConfig(const Config& config) {
	std::cout << "Global Config Settings:\n";
	std::cout << "  client_max_request_size: " << config.getClientMaxBodySize() << "\n";

	const std::map<int, std::string>& globalErrors = config.getErrorPages();
	for (std::map<int, std::string>::const_iterator it = globalErrors.begin(); it != globalErrors.end(); ++it) {
		std::cout << "  error_page " << it->first << ": " << it->second << "\n";
	}

	std::cout << "  index: " << join(config.getIndexPages()) << "\n";
	std::cout << "  accept_cgi: " << join(config.getAcceptCgi()) << "\n";
	std::cout << "  accept_method: " << config.getAcceptMethod() << "\n";
	std::cout << "  autoindex: " << (config.getAutoIndex() ? "on" : "off") << "\n";
	std::cout << "  chunk_size: " << (config.getBufferSize()) << "\n";
	std::cout << "  connection_timeout: " << config.getConnectionTimeout() << "\n";

	const std::vector<ServerConfig*>& servers = config.getServerConfigs();
	for (size_t i = 0; i < servers.size(); ++i) {
		const ServerConfig* server = servers[i];
		std::cout << "\nServer #" << (i + 1) << ":\n";
		std::cout << "  listen: " << server->getPort() << "\n";
		std::cout << "  server_name: " << join(server->getServerNames()) << "\n";
		std::cout << "  root: " << server->getRootFolder() << "\n";
		std::cout << "  index: " << join(server->getIndexPages()) << "\n";
		std::cout << "  accept_cgi: " << join(server->getAcceptCgi()) << "\n";
		std::cout << "  accept_method: " << server->getAcceptMethod() << "\n";
		std::cout << "  client_max_request_size: " << server->getClientMaxBodySize() << "\n";
		std::cout << "  autoindex: " << (server->getAutoIndex() ? "on" : "off") << "\n";

		const std::map<int, std::string>& serverErrors = server->getErrorPages();
		for (std::map<int, std::string>::const_iterator it = serverErrors.begin(); it != serverErrors.end(); ++it) {
			std::cout << "  error_page " << it->first << ": " << it->second << "\n";
		}

		const std::vector<Location *>& locations = server->getLocations();
		for (size_t j = 0; j < locations.size(); ++j) {
			const Location& loc = *locations[j];
			std::cout << "  Location: " << loc.getPath() << "\n";
			std::cout << "    root: " << loc.getRootFolder() << "\n";
			std::cout << "    index: " << join(loc.getIndexPages()) << "\n";
			std::cout << "    accept_method: " << loc.getAcceptMethod() << "\n";
			std::cout << "    accept_cgi: " << join(loc.getAcceptCgi()) << "\n";
			std::cout << "    client_max_request_size: " << loc.getClientMaxBodySize() << "\n";
			std::cout << "    autoindex: " << (loc.getAutoIndex() ? "on" : "off") << "\n";

			const std::map<int, std::string>& locErrors = loc.getErrorPages();
			for (std::map<int, std::string>::const_iterator it = locErrors.begin(); it != locErrors.end(); ++it) {
				std::cout << "    error_page " << it->first << ": " << it->second << "\n";
			}

			if (loc.isUploadPass())
				std::cout << "    upload_pass: enabled\n";

			if (loc.isRedirect())
				std::cout << "    redirect: " << loc.getRedirectCode() << " " << loc.getRedirectPath() << "\n";
		}
	}
}
