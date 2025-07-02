/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wpepping <wpepping@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 13:58:31 by auspensk          #+#    #+#             */
/*   Updated: 2025/07/02 20:00:53 by wpepping         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

Server::Server() { }

Server::Server(const Config &config) :
	_lastCleanup(std::time(0)),
	_config(&config) { }

Server::~Server() {
	Logger::debug() << "Cleaning up server" << std::endl;
	_cleanup();
}

Server &Server::operator=(Server const &other) {
	(void)other;
	return *this;
}

void Server::listen() throw(ChildProcessNeededException) {
	int					port;
	ListeningSocket		*socket;
	EventInfo			*eventInfo;
	std::vector<int>	portsDone;

	_epollInstance = epoll_create(1);
	SystemCallsUtilities::check_for_error(_epollInstance, "Failed to opening listening socket");

	for (size_t i = 0; i < _config->getServerConfigs().size(); i++) {
		port = _config->getServerConfigs()[i]->getPort();
		if (std::find(portsDone.begin(), portsDone.end(), port) == portsDone.end()) {
			socket = new ListeningSocket(port);
			eventInfo = new EventInfo(LISTENING_SOCKET, socket);
			socket->startListening();
			_listeningSockets.insert(std::pair<int, ListeningSocket*>(socket->getFd(), socket));
			_updateEvents(EPOLL_CTL_ADD, EPOLLIN, eventInfo, socket->getFd());
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
	size_t							size;
	int								readyFds;

	events.resize(100);
	while (true) {
		size = _connections.size() + _listeningSockets.size();
		if (events.capacity() < size)
			events.resize(size);
		readyFds = epoll_wait(_epollInstance, events.data(), size, TIMEOUT_CLEANUP_INTERVAL * 1000);
		Logger::detail() << "Epoll returned " << readyFds << " ready fds" << std::endl;
		SystemCallsUtilities::check_for_error(readyFds, "Critical error"); // We can't just exit here

		for(int i = 0; i < readyFds; ++i){
			Logger::detail() << "Going through ready list, i = " << i << ", event: " << events[i].events << std::endl;
			_handleEpollEvent(events[i].events, static_cast<EventInfo *>(events[i].data.ptr));
		}
		_iterateNonPollables(_nonPollableReadFds, EPOLLIN);
		_iterateNonPollables(_nonPollableWriteFds, EPOLLOUT);

		if (_invalidatedConnections.size() > 0)
			_cleanInvalidatedConnections();
		if (std::time(0) - _lastCleanup > TIMEOUT_CLEANUP_INTERVAL)
			_cleanIdleConnections();
	}
}

void Server::_iterateNonPollables(std::list<EventInfo *> fds, u_int32_t eventType) {
	EventInfo *eventInfo;
	for (std::list<EventInfo *>::iterator it = fds.begin(); it != fds.end(); ) {
		eventInfo = *it;
		++it;
		_handleEpollEvent(eventType, eventInfo);
	}
}

void Server::_handleEpollEvent(u_int32_t events, EventInfo *eventInfo) throw(ChildProcessNeededException) {
	Logger::detail() << "Epoll event type " << eventInfo->type << ", events: " << WebServUtils::getEpollEventNames(events) << std::endl;
	if (eventInfo->type == LISTENING_SOCKET)
		_handleIncomingConnection(eventInfo->listeningSocket);
	else if (eventInfo->conn->isInvalidated())
		return;
	else if (eventInfo->type == SOURCE)
		_handleSourceEvent(events, eventInfo);
	else if (eventInfo->type == SOCKET)
		_handleSocketEvent(events, eventInfo);
}

void Server::_handleSourceEvent(u_int32_t events, EventInfo *eventInfo) throw(ChildProcessNeededException) {
	if (events & EPOLLIN || events & EPOLLHUP)
		_readFromSource(*eventInfo);
	else if (events & EPOLLOUT)
		_writeToSource(*eventInfo);
	// else check for EPOLLERR and close connection
}

void Server::_handleSocketEvent(u_int32_t events, EventInfo *eventInfo) {
	if (events & EPOLLIN || events & EPOLLHUP)
		_readFromSocket(*eventInfo);
	else if (events & EPOLLOUT)
		_writeToSocket(*eventInfo);
	// else check for EPOLLERR and close connection
	eventInfo->conn->setLastActiveTime(std::time(0));
}

void Server::_handleIncomingConnection(ListeningSocket *listeningSocket) {
	struct sockaddr_storage	inc_addr;
	socklen_t				addr_size;
	int						new_fd;
	Connection				*inc_conn;
	EventInfo				*eventInfo;

	addr_size = sizeof(inc_addr);
	new_fd = accept(listeningSocket->getFd(), (struct sockaddr *)&inc_addr, &addr_size);

	Logger::info() << "Accepting incoming connection with fd " << new_fd << std::endl;

	inc_conn = new Connection(new_fd, listeningSocket->getPort());
	eventInfo = new EventInfo(SOCKET, inc_conn); // This may cause memory leaks, need to find a way to clean this up

	_connections.push_back(inc_conn);
	_updateEvents(EPOLL_CTL_ADD, EPOLLIN, eventInfo, new_fd);
}

void Server::_setupSource(Connection *conn) throw(ChildProcessNeededException, SourceAndRequestException) {
	//Logger::detail() <<"Request body: "<< conn->getRequestBody() << std::endl << std::endl;
	EventInfo *eventInfoSource;
	EventInfo *eventInfoSocket;

	conn->setupSource(*_config);
	conn->setResponse();
	eventInfoSource = new EventInfo(SOURCE, conn); // This may cause memory leaks, need to find a way to clean this up

	if (!conn->getSource()->_doneReading) {
		Logger::debug() << "Add source to epoll. fd: " << conn->getSource()->getFd() << std::endl;
		_updateEvents(EPOLL_CTL_ADD, EPOLLIN, eventInfoSource, conn->getSource()->getFd());
	}

	if (!conn->getSource()->_doneWriting) {
		Logger::debug() << "Add source write to epoll. fd: " << conn->getSource()->getWriteFd() << std::endl;
		_updateEvents(EPOLL_CTL_ADD, EPOLLOUT, eventInfoSource, conn->getSource()->getWriteFd());
	}

	if (conn->getSource()->isWriteWhenComplete())
		_updateEvents(EPOLL_CTL_DEL, EPOLLIN, eventInfoSource, conn->getSocketFd());
	else {
		eventInfoSocket = new EventInfo(SOCKET, conn); // This may cause memory leaks, need to find a way to clean this up
		_updateEvents(EPOLL_CTL_MOD, EPOLLOUT, eventInfoSocket, conn->getSocketFd());
	}
}

void Server::_readFromSocket(EventInfo &eventInfo) throw(ChildProcessNeededException) {
	Logger::detail() << "Server::_readFromSocket" << std::endl;

	try {
		eventInfo.conn->readFromSocket(_config->getBufferSize(), _config);
		if (eventInfo.conn->requestReady()) // finished reading request, create the source and the response
			_setupSource(eventInfo.conn);
	} catch (SourceAndRequestException &e) { // We need to clean up epoll and vectors here
		Logger::warning() << "SourceAndRequestException caught" << std::endl;
		eventInfo.conn->setupErrorPageSource(*_config, e.errorCode());
		eventInfo.conn->setResponse();
		_updateEvents(EPOLL_CTL_MOD, EPOLLOUT, &eventInfo, eventInfo.conn->getSocketFd());
	}
}

void Server::_writeToSocket(EventInfo &eventInfo) {
	Logger::detail() << "Server::_writeToSocket" << std::endl;
	eventInfo.conn->writeToSocket();
	if (eventInfo.conn->doneReadingSource() && eventInfo.conn->doneWritingSocket())
		_removeConnection(eventInfo.conn);
}

void Server::_readFromSource(EventInfo &eventInfoSource) {
	Logger::detail() << "Server::_readFromSource" << std::endl;
	if (!eventInfoSource.conn->getSource())
		return;
	eventInfoSource.conn->getSource()->readSource();
	if (eventInfoSource.conn->doneReadingSource()) {
		_updateEvents(EPOLL_CTL_DEL, EPOLLIN, &eventInfoSource, eventInfoSource.conn->getSourceFd());
		if (eventInfoSource.conn->getSource()->isWriteWhenComplete()) {
			EventInfo *eventInfoSocket = new EventInfo(SOCKET, eventInfoSource.conn); // This may cause memory leaks, need to find a way to clean this up
			_updateEvents(EPOLL_CTL_ADD, EPOLLOUT, eventInfoSocket, eventInfoSocket->conn->getSocketFd());
		}
	}
}

void Server::_writeToSource(EventInfo &eventInfo) {
	Logger::detail() << "Server::_writeToSource" << std::endl;
	eventInfo.conn->getSource()->writeSource();
	if (eventInfo.conn->doneWritingSource()) {
		Logger::debug() << "Done writing source" << std::endl;
		_updateEvents(EPOLL_CTL_DEL, EPOLLOUT, &eventInfo, eventInfo.conn->getSource()->getWriteFd());
	}
}

void Server::_updateEpoll(int action, u_int32_t events, EventInfo *eventInfo, int fd) {
	epoll_event event;
	epoll_event *event_ptr;

	if (action != EPOLL_CTL_DEL) {
		event_ptr = &event;
		event.events = events;
		event.data.ptr = eventInfo;
	} else {
		event_ptr = NULL;
	}

	if (epoll_ctl(_epollInstance, action, fd, event_ptr))
		Logger::warning() << "_updateEpoll failed" << std::endl;
}

void Server::_updateNonPollables(int action, u_int32_t events, EventInfo *eventInfo) {
	std::list<EventInfo *> &targetVector = events & EPOLLIN ? _nonPollableReadFds : _nonPollableWriteFds;
	std::list<EventInfo *> &otherVector = events & EPOLLIN ? _nonPollableWriteFds : _nonPollableReadFds;

	switch (action) {
		case EPOLL_CTL_DEL:
			WebServUtils::removeFromContainer(_nonPollableReadFds, eventInfo);
			WebServUtils::removeFromContainer(_nonPollableWriteFds, eventInfo);
			break;
		case EPOLL_CTL_MOD:
			WebServUtils::removeFromContainer(otherVector, eventInfo);
			targetVector.push_back(eventInfo);
			break;
		case EPOLL_CTL_ADD:
			targetVector.push_back(eventInfo);
			break;
	}
}

void Server::_updateEvents(int action, u_int32_t events, EventInfo *eventInfo, int fd) {
	bool pollable = eventInfo->type == LISTENING_SOCKET ||
		eventInfo->type == SOCKET ||
		(events & EPOLLIN && eventInfo->conn->getSource()->isPollableRead()) ||
		(events & EPOLLOUT && eventInfo->conn->getSource()->isPollableWrite());

	if (pollable)
		_updateEpoll(action, events, eventInfo, fd);
	else
		_updateNonPollables(action, events, eventInfo);

	if (eventInfo->conn && events & EPOLLIN)
		std::cout << ">>> Adding " << (pollable ? "" : "non-") << "pollable " <<
			(fd == eventInfo->conn->getSocketFd() ? "socket" : "source") << " fd to EPOLLIN: " << fd << std::endl;
	else if (events & EPOLLOUT)
		std::cout << ">>> Adding " << (pollable ? "" : "non-") << "pollable " <<
			(fd == eventInfo->conn->getSocketFd() ? "socket" : "source") << " fd to EPOLLOUT: " << fd << std::endl;
	else if (action == EPOLL_CTL_DEL)
		std::cout << ">>> Deleting " << (pollable ? "" : "non-") << "pollable fd from epoll: " << fd << std::endl;
}

ListeningSocket *Server::_findListeningSocket(int fd) {
	std::map<int, ListeningSocket*>::iterator needle;

	needle = _listeningSockets.find(fd);
	if (needle != _listeningSockets.end())
		return needle->second;
	else
		return NULL;
}

void Server::_cleanup() {
	for (std::map<int, ListeningSocket*>::iterator it = _listeningSockets.begin(); it != _listeningSockets.end(); ++it) {
		delete it->second;
	}
	for (std::vector<Connection*>::iterator it = _connections.begin(); it != _connections.end(); ++it) {
		delete (*it);
	}
	close(_epollInstance);
}

void Server::_removeConnection(Connection *conn) {
	conn->invalidate();
	_invalidatedConnections.push_back(conn);
}

void Server::_cleanInvalidatedConnections() {
	Connection* conn;

	for (size_t i = 0; i < _invalidatedConnections.size(); i++) {
		conn = _invalidatedConnections[i];
		Logger::debug() << "Closing connection!" << std::endl;
		_cleanConnection(conn);
	}
	_invalidatedConnections.clear();
}

void Server::_cleanIdleConnections() {
	time_t		currentTime = std::time(0);
	Connection	*conn;

	for (size_t i = 0; i < _connections.size(); i++) {
		conn = _connections[i];
		if (currentTime - conn->getLastActiveTime() > _config->getConnectionTimeout()) {
			Logger::info() << "Timeout on connection with socket fd " << conn->getSocketFd() << std::endl;
			_cleanConnection(conn);
		}
	}
	_lastCleanup = std::time(0);
}

void Server::_cleanEventInfo(int fd, uint32_t events) {
	std::map<int, EventInfo *>::iterator it = _eventInfoPtrs.find(fd);
	if (it != _eventInfoPtrs.end()) {
		_updateEvents(EPOLL_CTL_DEL, events, it->second, fd);
		delete it->second;
		_eventInfoPtrs.erase(it);
	}
}

void Server::_cleanConnection(Connection *conn) {
	_cleanEventInfo(conn->getSocketFd(), EPOLLIN);
	if (conn->getSource() && conn->getSource()->isPollableRead())
		_cleanEventInfo(conn->getSourceFd(), EPOLLIN);
	if (conn->getSource() && conn->getSource()->isPollableWrite())
		_cleanEventInfo(conn->getSource()->getWriteFd(), EPOLLOUT);
	delete conn;
	_connections.erase(
		std::remove(_connections.begin(), _connections.end(), conn),
		_connections.end()
	);
}
