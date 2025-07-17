/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigParseException.hpp                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wpepping <wpepping@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/09 16:03:39 by wpepping          #+#    #+#             */
/*   Updated: 2025/07/17 17:55:49 by wpepping         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <string>

class ConfigParseException : public std::exception {
public:
	ConfigParseException(const std::string &error);
	ConfigParseException(const ConfigParseException &src);
	ConfigParseException& operator=(const ConfigParseException &src);
	~ConfigParseException() throw();
	virtual const char* what() const throw();

private:
	std::string _error;
};
