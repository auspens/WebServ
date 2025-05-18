/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ListeningSocket.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wpepping <wpepping@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 16:44:56 by auspensk          #+#    #+#             */
/*   Updated: 2025/05/18 18:12:27 by wpepping         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ListeningSocket.hpp"

//now its private, keep in case it may be useful
ListeningSocket::ListeningSocket() : _listeningPort(3490) {
	init();
}

ListeningSocket::~ListeningSocket() { }

ListeningSocket::ListeningSocket(int port) : _listeningPort(port) {
	init();
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
		std::cout << gai_strerror(result) <<std::endl;
		std::exit(EXIT_FAILURE);
	}

	//create socket
	_fd = socket(_addrinfo->ai_family, _addrinfo->ai_socktype, _addrinfo->ai_protocol);
	SystemCallsUtilities::check_for_error(_fd);
}

void ListeningSocket::startListening() {
	// lose the "Address already in use" error message
	int yes=1;

	//bind
	SystemCallsUtilities::check_for_error(setsockopt(_fd, SOL_SOCKET,SO_REUSEADDR, &yes, sizeof(yes)));

	//listen
	SystemCallsUtilities::check_for_error(listen(_fd, BACKLOG));

	std::cout <<"Listening on port " << _listeningPort << std::endl;
}
