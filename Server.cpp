/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: auspensk <auspensk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 13:58:31 by auspensk          #+#    #+#             */
/*   Updated: 2025/06/06 18:22:34 by auspensk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

Server::Server() { }

Server::Server(const Config &config) : _config(&config) {
	init();
}

Server::~Server(){
	cleanup();
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
	int					port;
	ListeningSocket		*socket;
	std::vector<int>	portsDone;

	_epollInstance = epoll_create(1);
	SystemCallsUtilities::check_for_error(_epollInstance);

	for (size_t i = 0; i < _config->getServerConfigs().size(); i++) {
		port = _config->getServerConfigs()[i]->getPort();
		if (std::find(portsDone.begin(), portsDone.end(), port) == portsDone.end()) {
			socket = new ListeningSocket(port);
			socket->startListening();
			_listeningSockets.insert(std::pair<int, ListeningSocket*>(socket->getFd(), socket));
			_updateEpoll(EPOLL_CTL_ADD, EPOLLIN, NULL, socket->getFd());
			portsDone.push_back(port);
		}
	}

	_runEpollLoop();
}

void Server::_runEpollLoop() {
	//epoll wait parameters here: fd of epoll instance,
	//pointer to an array of epoll events (first element of the vector),
	//MAX_SIZE is set to the total amount of connections + 1(for listening socket)     // EW: What about CGI?
	//TIMEOUT is set to -1, so the wait blocks until one of monitored fds is ready
	std::vector<struct epoll_event>	events;
	int								size;
	int								readyFds;

	while (true) {
		size = _connections.size() + _listeningSockets.size();
		events.reserve(size);  // EW: is .resize() maybe safer?
		readyFds = epoll_wait(_epollInstance, &events[0], size, INFINITE_TIMEOUT); // EW: each epoll_event struct records: events on the fd (e.g. EPOLLIN) and data (ptr to connection)
		// std::cout << "Epoll returned " << readyFds << " ready fds" << std::endl;
		SystemCallsUtilities::check_for_error(readyFds);

		for(int i = 0; i < readyFds; ++i){
			// std::cout << "Going through ready list, i = " << i << std::endl;
			_handleSocketEvent(events[i]);
		}
	}
}

//create an interface that would contain a handleSocketEvent method and implement it for different classes.
//This would allow to avoid these if-else statements
void Server::_handleSocketEvent(struct epoll_event &event) {
	Connection *conn = static_cast<Connection*>(event.data.ptr);
	ListeningSocket *listeningSocket;

	listeningSocket = _findListeningSocket(event.data.fd);
	if (listeningSocket) {
		// std::cout << "epoll returned listening socket with fd " << event.data.fd << std::endl;
		_handleIncomingConnection(listeningSocket);
	}
	else if (event.events & EPOLLIN) {  // EW: maybe one also has to check for EPOLLHUP and EPOLLERR and if so close the connection
		if (conn->requestReady()) {
			// std::cout << "EPOLLIN and req is ready, so we read from source.." << std::endl;
			_readFromSource(*conn);
		}
		else {
			// std::cout << "EPOLLIN and req is not ready, so we read from socket.." << std::endl;
			_readFromSocket(conn);
		}
	}
	else if (event.events & EPOLLOUT) {
		// std::cout << "EPOLLOUT so we write to socket.." << std::endl;
		_writeToSocket(*conn);
	}
}

void Server::_handleIncomingConnection(ListeningSocket *listeningSocket) {
	struct sockaddr_storage	inc_addr;
	socklen_t				addr_size;
	int						new_fd;

	addr_size = sizeof(inc_addr);
	new_fd = accept(listeningSocket->getFd(), (struct sockaddr *)&inc_addr, &addr_size);

	Connection *inc_conn = new Connection(new_fd, listeningSocket->getPort());
	_connections.push_back(inc_conn);
	_updateEpoll(EPOLL_CTL_ADD, EPOLLIN, inc_conn, new_fd);
}

void Server::_readFromSocket(Connection *conn) {
	conn->readFromSocket();
	if (conn->requestReady())
	{
		// Reset parser was causing problems here
		//conn->resetParser();

		// finished reading request, create the source and the response
		std::cout <<"Request body: "<< conn->getRequestBody() <<std::endl;
		try {
			conn->setupSource(*_config);
			if (conn->getSource()->getType() == CGI)
				configureCGI(conn);
		}
		catch (Source::SourceException &e){
			// std::cout << e.what() << std::endl;
			// removeConnection(conn);
			conn->setupErrorPageSource(*_config, e.errorCode());
		}

		conn->setResponse();
		_updateEpoll(EPOLL_CTL_MOD, EPOLLOUT, conn, conn->getSocketFd());
		// if (conn->getSource()->getType() == CGI)
		// 	_updateEpoll(EPOLL_CTL_ADD, EPOLLIN, conn, conn->getSourceFd());
	}
}

void Server::_writeToSocket(Connection &conn) {
	conn.writeToSocket(); //if could not write returns 1
	//_updateEpoll(EPOLL_CTL_MOD, EPOLLIN, &conn, conn.getSocketFd());
	// conn.resetParser();

	// This seems wrong, if the source is done reading there may still be
	// buffered data that needs to be sent
	if (conn.doneReadingSource())
		removeConnection(&conn);
}

void Server::_readFromSource(Connection &conn) {
	if (!conn.getSource())
		return;
	conn.getSource()->readSource();
}

void Server::_updateEpoll(int action, int events, Connection *connection, int fd) {
	epoll_event event;

	event.events = events;
	if (connection)
		event.data.ptr = connection;
	else
		event.data.fd = fd;

	epoll_ctl(_epollInstance, action, fd, &event);
}

ListeningSocket *Server::_findListeningSocket(int fd) {
	std::map<int, ListeningSocket*>::iterator needle;

	needle = _listeningSockets.find(fd);
	if (needle != _listeningSockets.end())
		return needle->second;
	else
		return NULL;
}

void Server::cleanup() {
	for (std::map<int, ListeningSocket*>::iterator it = _listeningSockets.begin(); it != _listeningSockets.end(); ++it) {
    	delete it->second;
	}
    for (std::vector<Connection*>::iterator it = _connections.begin(); it != _connections.end(); ++it) {
    	delete (*it);
	}
	close(_epollInstance);
}

void Server::cleanupForFork(void* ctx) {
	std::cerr << "cleanup for child process" << std::endl; //not error, writing there due to dup
    Server* srv = static_cast<Server*>(ctx);
    srv->cleanup();
}

void Server::configureCGI(Connection* conn) {
	std::cout << "in configure CGI" << std::endl;
	CGISource *cgiptr = (CGISource *)conn->getSource();
	cgiptr->setPreExecCleanup(cleanupForFork, static_cast<void *>(this));
	_updateEpoll(EPOLL_CTL_ADD, EPOLLIN, conn, cgiptr->getPipeReadEnd());
	if (conn->getRequest().method == "POST") {
		int numbytes = write(cgiptr->getInputFd(), conn->getRequest().body.c_str(), conn->getRequest().body.length());
		std::cout << "method is POST! wrote " << numbytes << " bytes" << std::endl;
	}
	if (close(cgiptr->getInputFd()) != -1)
        std::cout << "Closed write end of input pipe" << std::endl;
}

void Server::removeConnection(Connection *conn) {
	_updateEpoll(EPOLL_CTL_DEL, EPOLLOUT, conn, conn->getSocketFd());
	std::cout << "Gonna delete connection!" << std::endl;
	delete conn;
	_connections.erase(
		std::remove(_connections.begin(), _connections.end(), conn),
		_connections.end()
	);
}
