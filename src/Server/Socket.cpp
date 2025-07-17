/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wpepping <wpepping@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 11:16:20 by auspensk          #+#    #+#             */
/*   Updated: 2025/07/17 16:37:21 by wpepping         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Socket.hpp"

Socket::Socket(int fd, struct addrinfo *addrinfo) : _fd(fd), _addrinfo(addrinfo) { }

Socket::Socket(int fd) : _fd(fd), _addrinfo(NULL) { }

Socket::~Socket() {
	Logger::debug() << "Closing socket fd " << _fd << std::endl;
	if (_addrinfo)
		freeaddrinfo(_addrinfo);
	close(_fd);
}

int Socket::getFd() const {
	return _fd;
}

Socket &Socket::operator=(const Socket &other){
	if (this != &other){
		_fd = other._fd;
		_addrinfo = other._addrinfo;
	}
	return *this;
}

Socket::Socket() : _fd(-1) {}

Socket::Socket(const Socket &other):
	_fd(other._fd)
	, _addrinfo(other._addrinfo) {}

void Socket::close_sock() {
	if (_fd > 0)
		close(_fd);
}
