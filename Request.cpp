/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wouter <wouter@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 14:09:57 by wouter            #+#    #+#             */
/*   Updated: 2025/04/26 18:01:14 by wouter           ###   ########.fr       */
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
