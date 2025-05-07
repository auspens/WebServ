#include "ConfigParseException.hpp"

ConfigParseException::ConfigParseException(std::string &error) {
	_error = error;
}

const char* ConfigParseException::what() const throw() {
	return _error.c_str();
}
