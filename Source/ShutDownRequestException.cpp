/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ShutDownRequestException.cpp                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wpepping <wpepping@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 10:24:45 by auspensk          #+#    #+#             */
/*   Updated: 2025/07/04 15:56:11 by wpepping         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ShutDownRequestException.hpp"

ShutDownRequestException::ShutDownRequestException(std::string error) throw():
	_error(error) { }

ShutDownRequestException::~ShutDownRequestException() throw() {}

const char *ShutDownRequestException::what() const throw() {
	return _error.c_str();
}
