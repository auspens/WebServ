/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: auspensk <auspensk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 13:58:31 by auspensk          #+#    #+#             */
/*   Updated: 2025/07/11 13:11:48 by auspensk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

Server::Server() { }

Server::Server(const Config &config) :
	_lastCleanup(std::time(0)),
	_config(&config),
	_shutDownFlag(false) { }

Server::~Server() {
	Logger::debug() << "Cleaning up server" << std::endl;
	_cleanup();
}

Server &Server::operator=(Server const &other) {
	(void)other;
	return *this;
}

void Server::listen() throw(IsChildProcessException) {
	int					port;
	ListeningSocket		*socket;
	std::vector<int>	portsDone;

	_epollInstance = epoll_create(1);
	SystemCallsUtilities::check_for_error(_epollInstance, "Failed to opening listening socket");

	for (size_t i = 0; i < _config->getServerConfigs().size(); i++) {
		port = _config->getServerConfigs()[i]->getPort();
		if (std::find(portsDone.begin(), portsDone.end(), port) == portsDone.end()) {
			socket = new ListeningSocket(port);
			socket->startListening();
			_listeningSockets.insert(std::pair<int, ListeningSocket*>(socket->getFd(), socket));
			_updateEvents(EPOLL_CTL_ADD, EPOLLIN, socket->getEventInfo(), socket->getFd());
			portsDone.push_back(port);
		}
	}

	_runEpollLoop();
}

// epoll wait parameters here: fd of epoll instance,
// pointer to an array of epoll events (first element of the vector),
// MAX_SIZE is set to the total amount of connections + listening sockets
// TIMEOUT is set to -1, so the wait blocks until one of monitored fds is ready
void Server::_runEpollLoop() throw(IsChildProcessException) {
	std::vector<struct epoll_event>	events;
	size_t							size;
	int								readyFds;
	int								timeoutInterval;

	events.resize(100);
	while (!_shutDownFlag) {
		size = _connections.size() + _listeningSockets.size();
		if (events.capacity() < size)
			events.resize(size);
		timeoutInterval = (_nonPollableReadFds.empty() &&
			_nonPollableWriteFds.empty() ? TIMEOUT_CLEANUP_INTERVAL * 1000 : 0);
		readyFds = epoll_wait(_epollInstance, events.data(), size, timeoutInterval);
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

void Server::_handleEpollEvent(u_int32_t events, EventInfo *eventInfo) throw(IsChildProcessException) {
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

void Server::_handleSourceEvent(u_int32_t events, EventInfo *eventInfo) throw(IsChildProcessException) {
	if (events & EPOLLIN || events & EPOLLHUP)
		_readFromSource(*eventInfo);
	else if (events & EPOLLOUT)
		_writeToSource(*eventInfo);
	// else check for EPOLLERR and close connection?
}

void Server::_handleSocketEvent(u_int32_t events, EventInfo *eventInfo) {
	if (events & EPOLLIN || events & EPOLLHUP)
		_readFromSocket(*eventInfo);
	else if (events & EPOLLOUT)
		_writeToSocket(*eventInfo);
	else if (events & EPOLLERR)
		_removeConnection(eventInfo->conn);
	eventInfo->conn->setLastActiveTime(std::time(0));
}

void Server::_handleIncomingConnection(ListeningSocket *listeningSocket) {
	struct sockaddr_storage	inc_addr;
	socklen_t				addr_size;
	int						new_fd;
	Connection				*inc_conn;

	addr_size = sizeof(inc_addr);
	new_fd = accept(listeningSocket->getFd(), (struct sockaddr *)&inc_addr, &addr_size);

	Logger::info() << "Accepting incoming connection with fd " << new_fd << std::endl;

	inc_conn = new Connection(new_fd, listeningSocket->getPort());

	_connections.push_back(inc_conn);
	_updateEvents(EPOLL_CTL_ADD, EPOLLIN, inc_conn->getSocketEventInfo(), new_fd);
}

void Server::_setupSource(Connection *conn) throw(IsChildProcessException, SourceAndRequestException) {
	//Logger::detail() <<"Request body: "<< conn->getRequestBody() << std::endl << std::endl;
	conn->setupSource(_shutDownFlag);
	conn->setResponse();

	if (!conn->doneReadingSource()) {
		Logger::debug() << "Add source to epoll. fd: " << conn->getSource()->getFd() << std::endl;
		_updateEvents(EPOLL_CTL_ADD, EPOLLIN, conn->getSourceEventInfo(), conn->getSource()->getFd());
	}

	if (!conn->doneWritingSource()) {
		Logger::debug() << "Add source write to epoll. fd: " << conn->getSource()->getWriteFd() << std::endl;
		_updateEvents(EPOLL_CTL_ADD, EPOLLOUT, conn->getSourceEventInfo(), conn->getSource()->getWriteFd());
	}

	if (conn->getSource()->isWriteWhenComplete())
		_updateEvents(EPOLL_CTL_DEL, EPOLLIN, conn->getSocketEventInfo(), conn->getSocketFd());
	else {
		_updateEvents(EPOLL_CTL_MOD, EPOLLOUT, conn->getSocketEventInfo(), conn->getSocketFd());
	}
}

void Server::_readFromSocket(EventInfo &eventInfo) throw(IsChildProcessException) {
	Logger::detail() << "Server::_readFromSocket" << std::endl;
	Connection *conn = eventInfo.conn;

	try {
		conn->readFromSocket(_config->getBufferSize(), _config);
		if (conn->requestReady()) // finished reading request, create the source and the response
			_setupSource(conn);
	} catch (SourceAndRequestException &e) {
		Logger::warning() << "SourceAndRequestException caught" << std::endl;
		conn->setupErrorPageSource(*_config, e.errorCode());
		conn->setResponse();
		_updateEvents(EPOLL_CTL_MOD, EPOLLOUT, &eventInfo, conn->getSocketFd());
	} catch (EmptyRequestException &e) {
		_removeConnection(conn);
	}
}

void Server::_writeToSocket(EventInfo &eventInfo) {
	Logger::detail() << "Server::_writeToSocket" << std::endl;
	Connection *conn = eventInfo.conn;

	conn->writeToSocket();
	if (conn->doneReadingSource() && conn->doneWritingSource() && conn->doneWritingSocket())
		_removeConnection(conn);
}

void Server::_readFromSource(EventInfo &eventInfo) {
	Logger::detail() << "Server::_readFromSource" << std::endl;
	Connection *conn = eventInfo.conn;

	if (!conn->getSource())
		return;
	conn->getSource()->readSource();
	if (conn->doneReadingSource()) {
		_updateEvents(EPOLL_CTL_DEL, EPOLLIN, &eventInfo, conn->getSourceFd());
		if (conn->getSource()->isWriteWhenComplete() && conn->doneWritingSource()) {
			_updateEvents(EPOLL_CTL_ADD, EPOLLOUT, conn->getSocketEventInfo(), conn->getSocketFd());
		}
	}
}

void Server::_writeToSource(EventInfo &eventInfo) {
	Logger::detail() << "Server::_writeToSource" << std::endl;
	Connection *conn = eventInfo.conn;

	conn->getSource()->writeSource();
	if (conn->doneWritingSource()) {
		Logger::debug() << "Done writing source" << std::endl;
		_updateEvents(EPOLL_CTL_DEL, EPOLLOUT, &eventInfo, conn->getSource()->getWriteFd());
		if (conn->getSource()->isWriteWhenComplete() && conn->doneReadingSource()) {
			_updateEvents(EPOLL_CTL_ADD, EPOLLOUT, conn->getSocketEventInfo(), conn->getSocketFd());
		}
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
	else
		Logger::debug() << "_updateEpoll action " << action << " events " << events << " fd " << fd << std::endl;
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

	if (events & EPOLLIN && action == EPOLL_CTL_ADD && eventInfo->conn)
		std::cout << ">>> Adding " << (pollable ? "" : "non-") << "pollable " <<
			(fd == eventInfo->conn->getSocketFd() ? "socket" : "source") << " fd to EPOLLIN: " << fd << std::endl;
	else if (events & EPOLLOUT && action == EPOLL_CTL_ADD)
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

void Server::_cleanConnection(Connection *conn) {
	_updateEvents(EPOLL_CTL_DEL, -1, conn->getSocketEventInfo(), conn->getSocketFd());

	if (conn->getSource() && conn->getSource()->isPollableRead())
		_updateEvents(EPOLL_CTL_DEL, EPOLLIN, conn->getSourceEventInfo(), conn->getSourceFd());
	else
		WebServUtils::removeFromContainer(_nonPollableReadFds, conn->getSourceEventInfo());

	if (conn->getSource() && conn->getSource()->isPollableWrite())
		_updateEvents(EPOLL_CTL_DEL, EPOLLOUT, conn->getSourceEventInfo(), conn->getSourceFd());
	else
		WebServUtils::removeFromContainer(_nonPollableWriteFds, conn->getSourceEventInfo());

	_connections.erase(
		std::remove(_connections.begin(), _connections.end(), conn),
		_connections.end()
	);
	delete conn;
}
