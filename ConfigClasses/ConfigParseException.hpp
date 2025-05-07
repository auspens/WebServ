#include <string>

class ConfigParseException : std::exception {
public:
	ConfigParseException(std::string &error);
	virtual const char* what() const throw();

private:
	std::string _error;
};
