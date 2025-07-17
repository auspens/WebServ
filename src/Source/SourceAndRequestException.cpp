/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SourceAndRequestException.cpp                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wpepping <wpepping@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 10:24:45 by auspensk          #+#    #+#             */
/*   Updated: 2025/07/17 17:58:15 by wpepping         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "SourceAndRequestException.hpp"

SourceAndRequestException::SourceAndRequestException (std::string error, int code) throw():
	_error(error),
	_code(code) { }

SourceAndRequestException::SourceAndRequestException(const SourceAndRequestException &src)
	: _error(src._error), _code(src._code) {}

SourceAndRequestException& SourceAndRequestException::operator=(const SourceAndRequestException &src) {
	if (this != &src) {
		_error = src._error;
		_code = src._code;
	}
	return *this;
}

SourceAndRequestException::~SourceAndRequestException() throw() {}

const char *SourceAndRequestException::what() const throw() {
	return _error.c_str();
}

int SourceAndRequestException::errorCode() const throw() {
	return _code;
}

