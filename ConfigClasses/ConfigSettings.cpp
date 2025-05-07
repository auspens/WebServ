#include "ConfigSettings.hpp"

ConfigSettings::ConfigSettings() : _client_max_body_size(-1), _autoindex(false) {}

ConfigSettings::~ConfigSettings() {}

ConfigSettings::ConfigSettings(ConfigSettings &src) {
	_client_max_body_size = src._client_max_body_size;
	_error_page = src._error_page;
	_index = src._index;
	_accept_cgi = src._accept_cgi;
	_accept_method = src._accept_method;
	_autoindex = src._autoindex;
}

ConfigSettings& ConfigSettings::operator=(ConfigSettings &src) {
	if (this != &src) {
		_client_max_body_size = src._client_max_body_size;
		_error_page = src._error_page;
		_index = src._index;
		_accept_cgi = src._accept_cgi;
		_accept_method = src._accept_method;
		_autoindex = src._autoindex;
	}
	return *this;
}

size_t ConfigSettings::getClientMaxBodySize() const {
	return _client_max_body_size;
}

const std::map<int, std::string>& ConfigSettings::getErrorPage() const {
	return _error_page;
}

const std::vector<std::string>& ConfigSettings::getIndex() const {
	return _index;
}

const std::vector<std::string>& ConfigSettings::getAcceptCgi() const {
	return _accept_cgi;
}

const std::vector<e_method>& ConfigSettings::getAcceptMethod() const {
	return _accept_method;
}

bool ConfigSettings::getAutoIndex() const {
	return _autoindex;
}

void ConfigSettings::parseClientMaxBodySize(std::ifstream &infile) {

}

void ConfigSettings::parseErrorPage(std::ifstream &infile) {

}

void ConfigSettings::parseIndex(std::ifstream &infile) {

}

void ConfigSettings::parseAcceptCgi(std::ifstream &infile) {

}

void ConfigSettings::parseAcceptMethod(std::ifstream &infile) {

}

void ConfigSettings::parseAutoIndex(std::ifstream &infile) {

}
