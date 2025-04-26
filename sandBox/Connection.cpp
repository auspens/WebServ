/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Connection.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wouter <wouter@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 16:46:34 by auspensk          #+#    #+#             */
/*   Updated: 2025/04/26 18:06:39 by wouter           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Connection.hpp"

Connection::Connection() {
}

Connection::Connection(int fd) : _socket(fd) { }

Connection::Connection(int fd, struct addrinfo *addrinfo)
		: _socket(fd, addrinfo) { }

Connection::Connection(const Connection &src) {
	(void)src;
}

Connection::~Connection() { }

Connection &Connection::operator=(const Connection &other) {
	(void)other;
	return *this;
};

int Connection::getSocketFd() const {
	return _socket.getFd();
}

int Connection::getSourceFd() const {
	return -1;
}

bool Connection::requestReady() const {
	return _request.isReady();
}

void Connection::readFromSocket(int buffer_size) {
	_request.read(_socket.getFd(), buffer_size);

	if (_request.isReady())
		prepSource();
}

void Connection::writeToSocket(int buffer_size) {
	const char	*str = "<html><body><header>Hello World!</header></body></html>\n";
	int			bytes_sent;

	(void)buffer_size;
	(void)bytes_sent;
	bytes_sent = send(_socket.getFd(), str, strlen(str), 0);
	_socket.close_sock();
}

void Connection::prepSource() {
}
