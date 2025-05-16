/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wpepping <wpepping@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 11:41:02 by auspensk          #+#    #+#             */
/*   Updated: 2025/05/15 19:06:46 by wpepping         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <string>
#include <map>
#include "ConfigParseException.hpp"
#include "ConfigSettings.hpp"
#include "ParseUtils.hpp"
#include "WebServUtils.hpp"

struct HttpRedirect {
	int code;
	std::string path;
};

class Location{
public:
	Location();
	Location(const std::string &path);
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
	const std::string &getIndex() const;
	const std::string &getRootFolder() const;
	const std::map<int, std::string> &getErorrPages() const;

private:
	bool 						_autoindex;
	bool						_uploadPass;
	bool						_isRedirect;
	HttpRedirect				_redirect;
	std::string 				_path;
	std::string 				_index;
	std::string 				_rootFolder;
	ConfigSettings				_configSettings;

	void _parseRoot(std::ifstream &file) throw(ConfigParseException);
	void _parseUploadPass(std::ifstream &file) throw(ConfigParseException);
	void _parseRedirect(std::ifstream &file) throw(ConfigParseException);
};
