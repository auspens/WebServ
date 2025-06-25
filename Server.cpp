/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wpepping <wpepping@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 13:58:31 by auspensk          #+#    #+#             */
/*   Updated: 2025/06/25 21:43:34 by wpepping         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

Server::Server() { }

Server::Server(const Config &config) :
	_lastCleanup(std::time(0)),
	_config(&config) { }

Server::~Server() {
	Logger::debug() << "Cleaning up server" << std::endl;
	cleanup();
}

Server &Server::operator=(Server const &other) {
	(void)other;
	return *this;
}

void Server::listen() throw(ChildProcessNeededException) {
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
			_updateEvents(EPOLL_CTL_ADD, EPOLLIN, NULL, socket->getFd());
			portsDone.push_back(port);
		}
	}

	_runEpollLoop();
}

// epoll wait parameters here: fd of epoll instance,
// pointer to an array of epoll events (first element of the vector),
// MAX_SIZE is set to the total amount of connections + listening sockets
// TIMEOUT is set to -1, so the wait blocks until one of monitored fds is ready
void Server::_runEpollLoop() throw(ChildProcessNeededException) {
	std::vector<struct epoll_event>	events;
	int								size;
	int								readyFds;
	Connection 						*conn;

	while (true) {
		size = _connections.size() + _listeningSockets.size();
		if (events.capacity() < static_cast<size_t>(size)) {// Do we really want to do this in the loop? Number of reserve calls if you're tranferring a large amount of data might be too much
  			events.reserve(size); //we can check if the current capacity is enough and reserve if not. Other option can be reserving at init and capping to configured size
								  // How about we reserve a fixed amount (something between 100 and 1000) and let it expand automatically if needed?
		}
		readyFds = epoll_wait(_epollInstance, &events[0], size, TIMEOUT_CLEANUP_INTERVAL * 1000); // EW: each epoll_event struct records: events on the fd (e.g. EPOLLIN) and data (ptr to connection)
		Logger::detail() << "Epoll returned " << readyFds << " ready fds" << std::endl;
		SystemCallsUtilities::check_for_error(readyFds);

		for(int i = 0; i < readyFds; ++i){
			Logger::detail() << "Going through ready list, i = " << i << ", event: " << events[i].events << std::endl;
			_handleSocketEvent(events[i].events, static_cast<Connection*>(events[i].data.ptr), events[i].data.fd);
		}
		for (std::list<Connection *>::iterator it = _nonPollableReadFds.begin(); it != _nonPollableReadFds.end(); ) {
			conn = *it;
			++it;
			_handleSocketEvent(EPOLLIN, conn, -1);
		}
		for (std::list<Connection *>::iterator it = _nonPollableWriteFds.begin(); it != _nonPollableWriteFds.end(); ) {
			conn = *it;
			it++;
			_handleSocketEvent(EPOLLOUT, conn, -1);
		}

		if (_invalidatedConnections.size() > 0)
			cleanInvalidatedConnections();
		if (std::time(0) - _lastCleanup > TIMEOUT_CLEANUP_INTERVAL)
			cleanIdleConnections();
	}
}

// create an interface that would contain a handleSocketEvent method and implement it for different classes.
// This would allow to avoid these if-else statements
void Server::_handleSocketEvent(u_int32_t events, Connection *conn, int fd) throw(ChildProcessNeededException) {
	ListeningSocket *listeningSocket;

	listeningSocket = _findListeningSocket(fd);
	if (listeningSocket) {
		Logger::debug() << "epoll returned listening socket with fd " << fd << std::endl;
		_handleIncomingConnection(listeningSocket);
		return;
	}
	else if (conn->isInvalidated())
		return;
	else if (events & EPOLLIN || events & EPOLLHUP) { // check for EPOLLERR and close connection
		if (!conn->requestReady()) {
			Logger::detail() << "EPOLLIN and req is not ready, so we read from socket.." << std::endl;
			_readFromSocket(conn);
		}
		else if (!conn->doneReadingSource()) {
			Logger::detail() << "EPOLLIN and req is ready, so we read from source.." << std::endl;
			_readFromSource(*conn);
		}
		else {
			Logger::detail() << "EPOLLHUP from socket, clean up connection" << std::endl;
			// removeConnection(conn);
		}
	}
	else if (events & EPOLLOUT) {
		if (conn->doneWritingSource()) {
			Logger::detail() << "EPOLLOUT and done writing source so we write to socket.." << std::endl;
			_writeToSocket(*conn);
		} else {
			Logger::detail() << "EPOLLOUT and still writing source so we write to source.." << std::endl;
			_writeToSource(*conn);
		}
	}
	conn->setLastActiveTime(std::time(0));
}

void Server::_handleIncomingConnection(ListeningSocket *listeningSocket) {
	struct sockaddr_storage	inc_addr;
	socklen_t				addr_size;
	int						new_fd;

	addr_size = sizeof(inc_addr);
	new_fd = accept(listeningSocket->getFd(), (struct sockaddr *)&inc_addr, &addr_size);

	Logger::info() << "Accepting incoming connection with fd " << new_fd << std::endl;

	Connection *inc_conn = new Connection(new_fd, listeningSocket->getPort());
	_connections.push_back(inc_conn);
	_updateEvents(EPOLL_CTL_ADD, EPOLLIN, inc_conn, new_fd);
}

void Server::_setupSource(Connection *conn) throw(ChildProcessNeededException, SourceAndRequestException) {
	//Logger::detail() <<"Request body: "<< conn->getRequestBody() << std::endl << std::endl;
	conn->setupSource(*_config);
	conn->setResponse();

	if (!conn->getSource()->_doneReading) {
		Logger::debug() << "Add source to epoll. fd: " << conn->getSource()->getFd() << std::endl;
		_updateEvents(EPOLL_CTL_ADD, EPOLLIN, conn, conn->getSource()->getFd());
	}

	if (!conn->getSource()->_doneWriting) {
		Logger::debug() << "Add source write to epoll. fd: " << conn->getSource()->getWriteFd() << std::endl;
		_updateEvents(EPOLL_CTL_ADD, EPOLLOUT, conn, conn->getSource()->getWriteFd());
	}

	if (conn->getSource()->isWriteWhenComplete())
		_updateEvents(EPOLL_CTL_DEL, -1, conn, conn->getSocketFd());
	else
		_updateEvents(EPOLL_CTL_MOD, EPOLLOUT, conn, conn->getSocketFd());
}

void Server::_readFromSocket(Connection *conn) throw(ChildProcessNeededException) {
	Logger::detail() << "Server::_readFromSocket" << std::endl;
	conn->readFromSocket(_config->getBufferSize(), _config);

	if (conn->requestReady()) { // finished reading request, create the source and the response
		try {
				_setupSource(conn);
		} catch (SourceAndRequestException &e) { // We need to clean up epoll and vectors here
			Logger::warning() << "SourceAndRequestException caught" << std::endl;
			conn->setupErrorPageSource(*_config, e.errorCode());
			conn->setResponse();
			_updateEvents(EPOLL_CTL_MOD, EPOLLOUT, conn, conn->getSocketFd());
		}
	}
}

void Server::_writeToSocket(Connection &conn) {
	Logger::detail() << "Server::_writeToSocket" << std::endl;
	conn.writeToSocket();
	if (conn.doneReadingSource() && conn.doneWritingSocket())
		removeConnection(&conn);
}

void Server::_readFromSource(Connection &conn) {
	Logger::detail() << "Server::_readFromSource" << std::endl;
	if (!conn.getSource())
		return;
	conn.getSource()->readSource();
	if (conn.doneReadingSource()) {
		_updateEvents(EPOLL_CTL_DEL, EPOLLIN, &conn, conn.getSourceFd());
		if (conn.getSource()->isWriteWhenComplete())
			_updateEvents(EPOLL_CTL_ADD, EPOLLOUT, &conn, conn.getSocketFd());
	}
}

void Server::_writeToSource(Connection &conn) {
	Logger::detail() << "Server::_writeToSource" << std::endl;
	conn.getSource()->writeSource();
	if (conn.doneWritingSource()) {
		Logger::debug() << "Done writing source" << std::endl;
		_updateEvents(EPOLL_CTL_DEL, EPOLLOUT, &conn, conn.getSource()->getWriteFd());
	}
}

void Server::_updateEpoll(int action, int events, Connection *connection, int fd) {
	epoll_event event;
	epoll_event *event_ptr;

	if (!(events & EPOLL_CTL_DEL)) {
		event_ptr = &event;
		event.events = events;
		if (connection)
			event.data.ptr = connection;
		else
			event.data.fd = fd;
	} else {
		event_ptr = NULL;
	}

	if (epoll_ctl(_epollInstance, action, fd, event_ptr))
		Logger::warning() << "_updateEpoll failed" << std::endl;
}

void Server::_updateEvents(int action, int events, Connection *connection, int fd) {
	if (!connection ||
		fd == connection->getSocketFd() ||
		(events == EPOLLIN && connection->getSource()->isPollableRead()) ||
		(events == EPOLLOUT && connection->getSource()->isPollableWrite())) {
		_updateEpoll(action, events, connection, fd);

		if (events == EPOLLOUT) {
			if (fd == connection->getSocketFd())
				std::cout << ">>> Adding socket fd to EPOLLOUT, fd: " << fd << std::endl;
			else
				std::cout << ">>> Adding source fd to EPOLLOUT, fd: " << fd << std::endl;
		}
		else if (connection && events == EPOLLIN) {
			if (fd == connection->getSocketFd())
				std::cout << ">>> Adding socket fd to EPOLLIN, fd: " << fd << std::endl;
			else
				std::cout << ">>> Adding source fd to EPOLLIN, fd: " << fd << std::endl;
		}
		else if (action == EPOLL_CTL_DEL)
			std::cout << ">>> Deleting fd " << fd << " from epoll" << std::endl;

		return;
	}

	std::list<Connection*> &targetVector = events == EPOLLIN ? _nonPollableReadFds : _nonPollableWriteFds;
	std::list<Connection*> &otherVector = events == EPOLLIN ? _nonPollableWriteFds : _nonPollableReadFds;

	switch (action) {
		case EPOLL_CTL_DEL:
			WebServUtils::removeFromContainer(_nonPollableReadFds, connection);
			WebServUtils::removeFromContainer(_nonPollableWriteFds, connection);
			break;
		case EPOLL_CTL_MOD:
			WebServUtils::removeFromContainer(otherVector, connection);
			targetVector.push_back(connection);
			break;
		case EPOLL_CTL_ADD:
			targetVector.push_back(connection);
			break;
	}
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

void Server::removeConnection(Connection *conn) {
	conn->invalidate();
	_invalidatedConnections.push_back(conn);
}

void Server::cleanInvalidatedConnections() {
	Connection* conn;

	for (size_t i = 0; i < _invalidatedConnections.size(); i++) {
		conn = _invalidatedConnections[i];
		Logger::debug() << "Closing connection!" << std::endl;
		cleanConnection(conn);
	}
	_invalidatedConnections.clear();
}

void Server::cleanIdleConnections() {
	time_t		currentTime = std::time(0);
	Connection	*conn;

	for (size_t i = 0; i < _connections.size(); i++) {
		conn = _connections[i];
		if (currentTime - conn->getLastActiveTime() > _config->getConnectionTimeout()) {
			Logger::info() << "Timeout on connection with socket fd " << conn->getSocketFd() << std::endl;
			cleanConnection(conn);
		}
	}
	_lastCleanup = std::time(0);
}

void Server::cleanConnection(Connection *conn) {
	_updateEvents(EPOLL_CTL_DEL, EPOLLIN, conn, conn->getSocketFd());
	if (conn->getSource() && conn->getSource()->isPollableRead())
		_updateEvents(EPOLL_CTL_DEL, EPOLLIN, conn, conn->getSourceFd());
	if (conn->getSource() && conn->getSource()->isPollableWrite())
		_updateEvents(EPOLL_CTL_DEL, EPOLLOUT, conn, conn->getSource()->getWriteFd());
	delete conn;
	_connections.erase(
		std::remove(_connections.begin(), _connections.end(), conn),
		_connections.end()
	);
}
