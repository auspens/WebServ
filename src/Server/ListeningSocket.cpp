/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ListeningSocket.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wpepping <wpepping@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 16:44:56 by auspensk          #+#    #+#             */
/*   Updated: 2025/07/17 17:34:31 by wpepping         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ListeningSocket.hpp"

//now its private, keep in case it may be useful
ListeningSocket::ListeningSocket() : _listeningPort(3490), _eventInfo(new EventInfo(LISTENING_SOCKET, this)) {
	init();
}

ListeningSocket::ListeningSocket(int port) : _listeningPort(port), _eventInfo(new EventInfo(LISTENING_SOCKET, this)) {
	init();
}

ListeningSocket::~ListeningSocket() {
	if (_fd > 0)
		Logger::debug() << "Closing listening socket: " << _fd << std::endl;
	delete _eventInfo;
}

ListeningSocket::ListeningSocket(const ListeningSocket &other):Socket(other) {
	_hints = other._hints;
	_eventInfo = other._eventInfo;
	_listeningPort = other._listeningPort;
}

ListeningSocket &ListeningSocket::operator=(const ListeningSocket &other) {
	if(this != &other){
		Socket::operator=(other);
		_hints = other._hints;
		_eventInfo = other._eventInfo;
		_listeningPort = other._listeningPort;
	}
	return *this;
}

void ListeningSocket::init() {
	//set address parameters
	std::memset(&_hints, 0, sizeof(_hints));
	_hints.ai_family = AF_UNSPEC;
	_hints.ai_socktype = SOCK_STREAM;
	_hints.ai_flags = AI_PASSIVE;
}

void ListeningSocket::startListening() throw(SocketOpenFailedException) {
	int result = getaddrinfo(NULL, WebServUtils::to_string(_listeningPort).c_str(), &_hints, &_addrinfo);

	if (result != 0) {
		Logger::error() << gai_strerror(result) <<std::endl;
		throw SocketOpenFailedException();
	}

	//create socket
	_fd = socket(_addrinfo->ai_family, _addrinfo->ai_socktype, _addrinfo->ai_protocol);
	if (_fd < 0)
		throw SocketOpenFailedException();

	// lose the "Address already in use" error message
	int yes=1;
	if (setsockopt(_fd, SOL_SOCKET,SO_REUSEADDR, &yes, sizeof(yes)) < 0)
		throw SocketOpenFailedException();

	//bind
	if (bind(_fd, _addrinfo->ai_addr, _addrinfo->ai_addrlen) < 0)
		throw SocketOpenFailedException();

	//listen
	if (listen(_fd, BACKLOG) < 0)
		throw SocketOpenFailedException();

	Logger::info() << "Listening on port " << _listeningPort << std::endl;
}

int ListeningSocket::getPort() {
	return _listeningPort;
}

EventInfo *ListeningSocket::getEventInfo() const {
	return _eventInfo;
}

const char *ListeningSocket::SocketOpenFailedException::what() const throw() {
	return "Failed to open listening socket";
}
