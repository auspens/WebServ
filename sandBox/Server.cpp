/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wouter <wouter@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 13:58:31 by auspensk          #+#    #+#             */
/*   Updated: 2025/04/25 18:02:25 by wouter           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

void Server::init() {
	_listen();
	_runEpollLoop();
}

void Server::_listen() {
	_listeningSocket = new ListeningSocket(_config.getHost(), config.getPort());
	_epollInstance = epoll_create(1);
	SystemCallsUtilities::check_for_error(_epollInstance);
	_listeningSocket->bindSocket();
	_listeningSocket->startListening();
	//add to epoll instance to listen for new incoming connections
	epoll_event *event = _listeningSocket->getEpollevent();
	event->events = EPOLLIN;
	event->data.fd = _listeningSocket->getFd();
	epoll_ctl(_epollInstance, EPOLL_CTL_ADD,
		_listeningSocket->getFd(), _listeningSocket->getEpollevent());
}

void Server::_runEpollLoop(){
	//epoll wait parameters here: fd of epoll instance,
	//pointer to an array of epoll events (first element of the vector),
	//MAX_SIZE is set to the total amount of connections + 1(for listening socket)
	//TIMEOUT is set to -1, so the wait blocks until one of monitored fds is ready
	std::vector<struct epoll_event> events;
	std::map<int, Connection>::iterator conn;

	while (true) {
		int size = _socketConnections.size() + _sourceConnections.size() + 1;
		events.reserve(size);
		int readyFds = epoll_wait(_epollInstance, &events[0],size, INFINITE_TIMEOUT);
		SystemCallsUtilities::check_for_error(readyFds);
		for(int i = 0; i < readyFds; ++i){
			if (events[i].data.fd == _listeningSocket->getFd())
				_handleIncomingConnection();
			else {
				conn = _socketConnections.find(events[i].data.fd);
				if (conn != _socketConnections.end()) {
					if (events[i].events & EPOLLIN)
						_readFromSocket(conn->second);
					if (events[i].events & EPOLLOUT)
						_writeToSocket(conn->second);
				} else {
					conn = _sourceConnections.find(events[i].data.fd);
					if (conn != _sourceConnections.end())
						_readFromSource(conn->second);
				}
			}
		}
	}
}

void Server::_readFromSocket(Connection &conn) {
	// if after read request.isReady is true, add connection to _sourceConnections and update events in epoll_ctl
}

Server::Server() {
	ServerConfig config("localhost", 80);
	_config = config;
}

Server::Server(ServerConfig &config)	: _config(config), _epollInstance(epoll_create(1)) {
	SystemCallsUtilities::check_for_error(_epollInstance);
}

Server::~Server(){
	close(_epollInstance);
}

Server &Server::operator=(Server const &other){return *this;}
