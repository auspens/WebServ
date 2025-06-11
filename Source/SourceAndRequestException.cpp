/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SourceAndRequestException.cpp                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: auspensk <auspensk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 10:24:45 by auspensk          #+#    #+#             */
/*   Updated: 2025/06/11 10:26:49 by auspensk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "SourceAndRequestException.hpp"

SourceAndRequestException::SourceAndRequestException(std::string error, int code)throw(): _error(error), _code(code){}
SourceAndRequestException::~SourceAndRequestException()throw(){}
const char *SourceAndRequestException::what() const throw(){
	return _error.c_str();
}
int SourceAndRequestException::errorCode() const throw(){
	return _code;
}

