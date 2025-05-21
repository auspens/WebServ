/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: auspensk <auspensk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 13:58:31 by auspensk          #+#    #+#             */
/*   Updated: 2025/05/21 14:29:25 by auspensk         ###   ########.fr       */
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
	std::cout << "port in server: " << _config.getPort() << std::endl;
	_listeningSockets.push_back(new ListeningSocket(_config.getPort(), _config.getHost()));
	_epollInstance = epoll_create(1);

	SystemCallsUtilities::check_for_error(_epollInstance);
	_listeningSockets[0]->bindSocket();
	_listeningSockets[0]->startListening();

	//add to epoll instance to listen for new incoming connections
	_updateEpoll(EPOLL_CTL_ADD, EPOLLIN, NULL, _listeningSockets[0]->getFd());
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
		size = _connections.size() + _listeningSockets.size();
		events.reserve(size);
		readyFds = epoll_wait(_epollInstance, &events[0], size, INFINITE_TIMEOUT);
		SystemCallsUtilities::check_for_error(readyFds);

		for(int i = 0; i < readyFds; ++i){
			_handleSocketEvent(events[i]);
		}
	}
}

//create an interface that would contain a handleSocketEvent method and implement it for different classes.
//This would allow to avoid these if-else statements
void Server::_handleSocketEvent(struct epoll_event &event) {
	Connection *conn = static_cast<Connection*>(event.data.ptr);

	// if listening socket, handle incoming connection
	if (!conn)
		_handleIncomingConnection(*_listeningSockets[0]);
	else if (event.events & EPOLLIN) {
		if (conn->requestReady())
			_readFromSource(*conn);
		else
			_readFromSocket(conn);
	}
	else if (event.events & EPOLLOUT)
		_writeToSocket(*conn);
}

void Server::_handleIncomingConnection(ListeningSocket &listeningSocket) {
	struct sockaddr_storage	inc_addr;
	socklen_t				addr_size;
	int						new_fd;

	addr_size = sizeof(inc_addr);
	new_fd = accept(listeningSocket.getFd(), (struct sockaddr *)&inc_addr, &addr_size);

	Connection *inc_conn = new Connection(new_fd);
	_connections.push_back(inc_conn);
	_updateEpoll(EPOLL_CTL_ADD, EPOLLIN, inc_conn, new_fd);
}

void Server::_readFromSocket(Connection *conn) {
	conn->readFromSocket();
	if (conn->requestReady())
	{
		//conn->resetParser();
		try {
			// finished reading request, create the source and the response
			std::cout << "Root folder for server: " << (_config).getRootFolder() << std::endl;
			conn->setSource(Source::getNewSource(conn->getTarget(), _config));
			if (conn->getSource()->getType() == CGI) {
				_updateEpoll(EPOLL_CTL_ADD, EPOLLIN, conn, conn->getSourceFd());
			}
			conn->setResponse(conn->getSource());
		}
		catch (Source::SourceException &e){
			std::cout << e.what() << std::endl;
		}
		_updateEpoll(EPOLL_CTL_MOD, EPOLLOUT, conn, conn->getSocketFd());
	}
}

void Server::_writeToSocket(Connection &conn) {
	conn.writeToSocket();
	//when the response is sent, then (1) if keep alive - then epoll_ctl to EPOLLIN, or (2) close connection
}

void Server::_readFromSource(Connection &conn) {
	if (!conn.getSource())
		return;
	conn.getSource()->readSource();
}

void Server::_updateEpoll(int action, int events, Connection *connection, int fd) {
	epoll_event event;

	event.events = events;
	event.data.ptr = connection;
	epoll_ctl(_epollInstance, action, fd, &event);
}
