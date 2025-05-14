/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigParseException.cpp                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wouter <wouter@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/09 16:03:49 by wpepping          #+#    #+#             */
/*   Updated: 2025/05/14 17:50:07 by wouter           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ConfigParseException.hpp"

ConfigParseException::ConfigParseException(const std::string &error) {
	_error = error;
}

ConfigParseException::~ConfigParseException() { }

const char* ConfigParseException::what() const throw() {
	return _error.c_str();
}
