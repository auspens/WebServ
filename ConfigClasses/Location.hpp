/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wpepping <wpepping@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 11:41:02 by auspensk          #+#    #+#             */
/*   Updated: 2025/05/09 19:23:52 by wpepping         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <string>
#include <map>
#include "ConfigParseException.hpp"
#include "ParseUtils.hpp"

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
	const std::map<int, std::string> &getErorrPagesLocal() const;

private:
	std::string 				_path;
	bool 						_autoindex;
	std::string 				_index;
	std::pair<int, std::string> _redirect;
	std::map<int, std::string>	_errorPagesLocal;
	ConfigSettings				_configSettings;

	void _parseRoot(std::ifstream &file) throw(ConfigParseException);
};
