/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wpepping <wpepping@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 11:41:02 by auspensk          #+#    #+#             */
/*   Updated: 2025/05/15 14:58:00 by wpepping         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <string>
#include <map>
#include "ConfigParseException.hpp"
#include "ConfigSettings.hpp"
#include "ParseUtils.hpp"
#include "WebServUtils.hpp"

class Location{
public:
	Location();
	Location(const std::string &path);
	~Location();
	Location(const Location &src);
	Location &operator =(const Location &other);

	void parse(std::ifstream &file) throw(ConfigParseException);

	const std::string &getPath() const;
	const std::string &getIndex() const;
	bool autoindexOn() const;
	const std::pair<int, std::string> &getRedirect() const;
	const std::map<int, std::string> &getErorrPages() const;

private:
	bool 						_autoindex;
	std::string 				_path;
	std::string 				_index;
	std::string 				_rootFolder;
	std::pair<int, std::string> _redirect;
	ConfigSettings				_configSettings;

	void _parseRoot(std::ifstream &file) throw(ConfigParseException);
};
