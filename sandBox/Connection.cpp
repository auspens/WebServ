/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Connection.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: auspensk <auspensk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 16:46:34 by auspensk          #+#    #+#             */
/*   Updated: 2025/04/23 17:17:05 by auspensk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Connection.hpp"

Connection::Connection(int fd, struct addrinfo *addrinfo)
		:_socket(fd, addrinfo){
	std::memset(_epollEvent, 0, sizeof(*_epollEvent));
}

struct epoll_event *Connection::getEpollevent(){return _epollEvent;}

Connection::~Connection(){};
Connection &Connection::operator=(const Connection &other){return *this;};
