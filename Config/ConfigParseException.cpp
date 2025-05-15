/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigParseException.cpp                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wpepping <wpepping@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/09 16:03:49 by wpepping          #+#    #+#             */
/*   Updated: 2025/05/15 15:03:15 by wpepping         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ConfigParseException.hpp"

ConfigParseException::ConfigParseException(const std::string &error) {
	_error = error;
}

ConfigParseException::~ConfigParseException() throw() { }

const char* ConfigParseException::what() const throw() {
	return _error.c_str();
}
