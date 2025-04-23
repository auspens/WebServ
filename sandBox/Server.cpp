/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: auspensk <auspensk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 13:58:31 by auspensk          #+#    #+#             */
/*   Updated: 2025/04/23 17:50:56 by auspensk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

void Server::runEpollLoop(){
	//epoll wait parameters here: fd of epoll instance,
	//pointer to an array of epoll events (first element of the vector),
	//MAX_SIZE is set to the total amount of connections + 1(for listening socket)
	//TIMEOUT is set to -1, so the wait blocks until one of monitored fds is ready
	while (true){
		int size = _connections.size() + 1;
		_events.reserve(size);
		int readyFds = epoll_wait(_epollInstance,&_events[0],size, -1);
		check_for_error(readyFds);
		for(int i = 0; i < readyFds; ++i){
//here: if fd is listening socket - accept the connection
//else - get the request/send info from/to the connection
		}
	}
}

void Server::openToConnections(){
	_listeningSocket.bindSocket();
	_listeningSocket.startListening();
	//add to epoll instance to listen for new incoming connections
	epoll_event *event = _listeningSocket.getEpollevent();
	event->events = EPOLLIN;
	epoll_ctl(_epollInstance, EPOLL_CTL_ADD,
		_listeningSocket.getFd(), _listeningSocket.getEpollevent());
}


Server::Server(std::string const &port, std::string const &host)
		:_listeningSocket(port, host), _epollInstance(0){
			_epollInstance = epoll_create(1);
			check_for_error(_epollInstance);
}
Server::Server(): _listeningSocket("localhost", "3490"){
	_epollInstance = epoll_create(1);
	check_for_error(_epollInstance);
}
Server::~Server(){
	close(_epollInstance);
}
Server &Server::operator=(Server const &other){return *this;}
