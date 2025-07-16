/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: auspensk <auspensk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 11:16:20 by auspensk          #+#    #+#             */
/*   Updated: 2025/07/16 15:56:52 by auspensk         ###   ########.fr       */
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

Socket::Socket() {}

Socket::Socket(const Socket &other):
	_fd(other._fd)
	, _addrinfo(other._addrinfo) {}

void Socket::close_sock() {
	close(_fd);
}
