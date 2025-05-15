/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wpepping <wpepping@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 13:58:31 by auspensk          #+#    #+#             */
/*   Updated: 2025/05/15 15:27:41 by wpepping         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

Server::Server(const ServerConfig &config) : _config(config) {
	init();
}

Server::~Server(){
	close(_epollInstance);
}

Server &Server::operator=(Server const &other){
	(void)other;
	return *this;
}

void Server::init() {
	_listen();
	_runEpollLoop();
}

void Server::_listen() {
	_listeningSocket = new ListeningSocket(_config.getPort(), _config.getHost());
	_epollInstance = epoll_create(1);

	SystemCallsUtilities::check_for_error(_epollInstance);
	_listeningSocket->bindSocket();
	_listeningSocket->startListening();

	//add to epoll instance to listen for new incoming connections
	_updateEpoll(EPOLL_CTL_ADD, EPOLLIN, _listeningSocket->getFd());
	_runEpollLoop();
}

void Server::_runEpollLoop() {
	//epoll wait parameters here: fd of epoll instance,
	//pointer to an array of epoll events (first element of the vector),
	//MAX_SIZE is set to the total amount of connections + 1(for listening socket)
	//TIMEOUT is set to -1, so the wait blocks until one of monitored fds is ready
	std::vector<struct epoll_event>	events;
	int								size;
	int								readyFds;

	while (true) {
		size = _socketConnections.size() + _sourceConnections.size() + 1;
		events.reserve(size);
		readyFds = epoll_wait(_epollInstance, &events[0], size, INFINITE_TIMEOUT);
		SystemCallsUtilities::check_for_error(readyFds);

		for(int i = 0; i < readyFds; ++i){
			_handleSocketEvent(events[i]);
		}
	}
}

//create an interface that would contain a handleSocketEven method and implement it for different classes.
//This would allow to avoid these if-else statements
void Server::_handleSocketEvent(struct epoll_event &event) {
	std::map<int, Connection *>::iterator conn;

	// if listening socket, handle incoming connection
	if (event.data.fd == _listeningSocket->getFd())
		_handleIncomingConnection(*_listeningSocket);
	else {
		// if source connection, read from source file or cgi
		conn = _sourceConnections.find(event.data.fd);
		if (conn != _sourceConnections.end()){
			_readFromSource(*conn->second);
		}
		else {
			// socket connection, read from or write to socket based on event type
			conn = _socketConnections.find(event.data.fd);
			if (conn != _socketConnections.end()) {
				if (event.events & EPOLLIN)
					_readFromSocket(*conn->second);
				if (event.events & EPOLLOUT){
					_readFromSource(*conn->second);
					_writeToSocket(*conn->second);
				}
			}
		}
	}
}

void Server::_handleIncomingConnection(ListeningSocket &listeningSocket) {
	struct sockaddr_storage	inc_addr;
	socklen_t				addr_size;
	int						new_fd;

	addr_size = sizeof(inc_addr);
	new_fd = accept(listeningSocket.getFd(), (struct sockaddr *)&inc_addr, &addr_size);

	Connection *inc_conn = new Connection(new_fd);
	_socketConnections.insert(std::make_pair(new_fd, inc_conn));
	_updateEpoll(EPOLL_CTL_ADD, EPOLLIN, new_fd);
}

void Server::_readFromSocket(Connection &conn) {
	conn.readFromSocket(_config.getBufferSize());
	if (conn.requestReady())
	{
		conn.resetParser();
	// finished reading request, create the source
		try {
			conn.setSource(Source::getNewSource(conn.getTarget(), _config));
			if (conn.getSource()->getType() == CGI) {
				_sourceConnections.insert(std::make_pair(conn.getSourceFd(), &conn));
				_updateEpoll(EPOLL_CTL_ADD, EPOLLIN, conn.getSourceFd());
		}
		}
		catch (Source::SourceException &e){
			std::cout << e.what() << std::endl;
		}
		_updateEpoll(EPOLL_CTL_MOD, EPOLLOUT, conn.getSocketFd());
	}
}

void Server::_writeToSocket(Connection &conn) {
	conn.writeToSocket(_config.getBufferSize());
}

void Server::_readFromSource(Connection &conn) {
	if (!conn.getSource())
		return;
	conn.getSource()->readSource();
	conn.generateResponse();
}

void Server::_updateEpoll(int action, int events, int fd) {
	epoll_event event;

	event.events = events;
	event.data.fd = fd;
	epoll_ctl(_epollInstance, action, fd, &event);
}
