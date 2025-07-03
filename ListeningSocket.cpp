/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ListeningSocket.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wouter <wouter@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 16:44:56 by auspensk          #+#    #+#             */
/*   Updated: 2025/07/03 19:00:50 by wouter           ###   ########.fr       */
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
	Logger::info() << "Closing listening socket: ";
	delete _eventInfo;
}

void ListeningSocket::init() {
	int result;

	//set address parameters
	std::memset(&_hints, 0, sizeof(_hints));
	_hints.ai_family = AF_UNSPEC;
	_hints.ai_socktype = SOCK_STREAM;
	_hints.ai_flags = AI_PASSIVE;
	result = getaddrinfo(NULL, WebServUtils::to_string(_listeningPort).c_str(), &_hints, &_addrinfo);

	if (result != 0){
		Logger::error() << gai_strerror(result) <<std::endl;
		std::exit(EXIT_FAILURE);
	}

	//create socket
	_fd = socket(_addrinfo->ai_family, _addrinfo->ai_socktype, _addrinfo->ai_protocol);
	SystemCallsUtilities::check_for_error(_fd, "Failed to opening listening socket");
}

void ListeningSocket::startListening() {
	// lose the "Address already in use" error message
	int yes=1;
	SystemCallsUtilities::check_for_error(setsockopt(_fd, SOL_SOCKET,SO_REUSEADDR, &yes, sizeof(yes)), "Failed to opening listening socket");

	//bind
	SystemCallsUtilities::check_for_error(bind(_fd, _addrinfo->ai_addr, _addrinfo->ai_addrlen), "Failed to opening listening socket");

	//listen
	SystemCallsUtilities::check_for_error(listen(_fd, BACKLOG), "Failed to opening listening socket");

	Logger::info() << "Listening on port " << _listeningPort << std::endl;
}

int ListeningSocket::getPort() {
	return _listeningPort;
}

EventInfo *ListeningSocket::getEventInfo() const {
	return _eventInfo;
}
