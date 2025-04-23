/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ListeningSocket.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: auspensk <auspensk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 16:44:56 by auspensk          #+#    #+#             */
/*   Updated: 2025/04/23 16:59:31 by auspensk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ListeningSocket.hpp"

ListeningSocket::ListeningSocket(std::string const &port, std::string const &host){
//set address parameters
	std::memset(&_hints, 0, sizeof(_hints));
	_hints.ai_family = AF_UNSPEC;
	_hints.ai_socktype = SOCK_STREAM;
	int s = getaddrinfo(host.c_str(), port.c_str(), &_hints, &_servinfo);
	if (s != 0){
		std::cout << gai_strerror(s) <<std::endl;
		std::exit(EXIT_FAILURE);
	}
//create socket
	_fd = socket(_servinfo->ai_family, _servinfo->ai_socktype, _servinfo->ai_protocol);
	check_for_error(_fd);
//memset epoll event struct
	std::memset(_epollEvent, 0, sizeof(*_epollEvent));
}

void ListeningSocket::bindSocket(){
// lose the pesky "Address already in use" error message
	int yes=1;
	check_for_error(setsockopt(_fd,SOL_SOCKET,SO_REUSEADDR,&yes,sizeof(yes)));
//bind
	check_for_error(bind(_fd, _servinfo->ai_addr, _servinfo->ai_addrlen));
}

void ListeningSocket::startListening(){
	check_for_error(listen(_fd, BACKLOG));
	std::cout <<"Listening on socket" << std::endl;
}


//now its private, keep in case it may be useful
ListeningSocket::ListeningSocket(){
		//set address parameters
		std::memset(&_hints, 0, sizeof(_hints));
		_hints.ai_family = AF_UNSPEC;
		_hints.ai_socktype = SOCK_STREAM;
		_hints.ai_flags = AI_PASSIVE;
		int s = getaddrinfo(NULL, "3490", &_hints, &_servinfo);
		if (s != 0){
			std::cout << gai_strerror(s) <<std::endl;
			std::exit(EXIT_FAILURE);
		}
	//create socket
		_fd = socket(_servinfo->ai_family,
			_servinfo->ai_socktype, _servinfo->ai_protocol);
		check_for_error(_fd);
}

