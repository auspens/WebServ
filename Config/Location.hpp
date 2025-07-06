/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wouter <wouter@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 11:41:02 by auspensk          #+#    #+#             */
/*   Updated: 2025/07/06 18:36:02 by wouter           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <string>
#include <map>
#include "ConfigParseException.hpp"
#include "ConfigSettings.hpp"
#include "ParseUtils.hpp"
#include "WebServUtils.hpp"

class ServerConfig;

struct HttpRedirect {
	int code;
	std::string path;
};

class Location{
public:
	Location();
	Location(const ServerConfig &serverConfig);
	~Location();
	Location(const Location &src);
	Location &operator =(const Location &other);

	void parse(std::ifstream &file) throw(ConfigParseException);

	bool autoindexOn() const;
	bool isUploadPass() const;
	bool isRedirect() const;
	int getRedirectCode() const;
	const std::string &getRedirectPath() const;
	const std::string &getPath() const;
	const std::string &getRootFolder() const;
	size_t getClientMaxBodySize() const;
	const std::map<int, std::string>& getErrorPages() const;
	const std::vector<std::string>& getIndexPages() const;
	const std::vector<std::string>& getAcceptCgi() const;
	int getAcceptMethod() const;
	bool getAutoIndex() const;

private:
	bool 						_autoindex;
	bool						_uploadPass;
	bool						_isRedirect;
	HttpRedirect				_redirect;
	std::string 				_path;
	std::string 				_index;
	std::string 				_rootFolder;
	ConfigSettings				_configSettings;
	const ServerConfig			*_serverConfig;

	void _parseRoot(std::ifstream &file) throw(ConfigParseException);
	void _parseUploadPass(std::ifstream &file) throw(ConfigParseException);
	void _parseRedirect(std::ifstream &file) throw(ConfigParseException);
};
