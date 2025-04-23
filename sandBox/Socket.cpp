/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: auspensk <auspensk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 11:16:20 by auspensk          #+#    #+#             */
/*   Updated: 2025/04/23 17:10:59 by auspensk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Socket.hpp"


Socket::Socket(int fd, struct addrinfo *addrinfo)
	:_fd(fd), _servinfo(addrinfo){}

Socket::~Socket(){
	freeaddrinfo(_servinfo);
	close(_fd);
}

int Socket::getFd(){return _fd;}

Socket &Socket::operator=(const Socket &other){return *this;}
Socket::Socket(){}
Socket::Socket(const Socket&src){}
