/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wpepping <wpepping@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 14:09:57 by wouter            #+#    #+#             */
/*   Updated: 2025/06/17 18:38:59 by wpepping         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"

Request::Request() : _requestType("GET"), _target("/index.html"), _isReady(true) {}

Request::~Request() {}

bool Request::isReady() const {
	return _isReady;
}

void Request::read(int fd, int buffer_size) {
	(void)fd;
	(void)buffer_size;
	_isReady = true;
}

const std::string &Request::getTarget()const { return _target;}
