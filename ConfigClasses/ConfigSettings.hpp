#include <map>
#include <string>
#include <vector>
#include "ConfigParseException.hpp"

enum e_method {
	GET,
	POST,
	DELETE
};

class ConfigSettings {
public:
	ConfigSettings();
	~ConfigSettings();
	ConfigSettings(ConfigSettings &src);
	ConfigSettings &operator=(ConfigSettings &src);

	void parseClientMaxBodySize(std::ifstream &infile);
	void parseErrorPage(std::ifstream &infile);
	void parseIndex(std::ifstream &infile);
	void parseAcceptCgi(std::ifstream &infile);
	void parseAcceptMethod(std::ifstream &infile);
	void parseAutoIndex(std::ifstream &infile);

	size_t								getClientMaxBodySize()	const;
	const std::map<int, std::string>	&getErrorPage()			const;
	const std::vector<std::string>		&getIndex()				const;
	const std::vector<std::string>		&getAcceptCgi()			const;
	const std::vector<e_method>			&getAcceptMethod()		const;
	bool								getAutoIndex()			const;

private:
	size_t						_client_max_body_size;
	std::map<int, std::string>	_error_page;
	std::vector<std::string>	_index;
	std::vector<std::string>	_accept_cgi;
	std::vector<e_method>		_accept_method;
	bool						_autoindex;
};
