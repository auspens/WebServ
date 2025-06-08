/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wouter <wouter@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 11:16:20 by auspensk          #+#    #+#             */
/*   Updated: 2025/06/08 13:41:17 by wouter           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Socket.hpp"

Socket::Socket(int fd, struct addrinfo *addrinfo) : _fd(fd), _addrinfo(addrinfo) { }

Socket::Socket(int fd) : _fd(fd), _addrinfo(NULL) { }

Socket::~Socket() {
	std::cout << "Closing socket fd " << _fd << std::endl;
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
