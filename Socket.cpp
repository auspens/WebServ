/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wpepping <wpepping@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 11:16:20 by auspensk          #+#    #+#             */
/*   Updated: 2025/06/18 17:10:10 by wpepping         ###   ########.fr       */
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
	(void)other;
	return *this;
}

Socket::Socket() {}

Socket::Socket(const Socket &other) {
	(void)other;
}

void Socket::close_sock() {
	close(_fd);
}
