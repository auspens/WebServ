/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wpepping <wpepping@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 13:53:34 by auspensk          #+#    #+#             */
/*   Updated: 2025/06/25 19:33:24 by wpepping         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <algorithm>
#include <ctime>
#include <list>
#include <map>
#include <vector>
#include "ChildProcessNeededException.hpp"
#include "Config.hpp"
#include "Connection.hpp"
#include "Constants.hpp"
#include "ListeningSocket.hpp"
#include "Logger.hpp"

#define INFINITE_TIMEOUT -1

class Server {
	public:
		Server(const Config &config);
		~Server();

		void listen() throw(ChildProcessNeededException);

	private:
		int									_epollInstance;
		time_t								_lastCleanup;
		const Config						*_config;
		std::map<int, ListeningSocket *>	_listeningSockets;
		std::vector<Connection *>			_connections;
		std::vector<Connection *>			_invalidatedConnections;
		std::list<Connection *>				_nonPollableReadFds;
		std::list<Connection *>				_nonPollableWriteFds;

		Server();
		Server(Server const &src);
		Server &operator=(Server const &other);

		void _runEpollLoop() throw(ChildProcessNeededException);
		void _handleSocketEvent(u_int32_t event, Connection *conn, int fd) throw(ChildProcessNeededException);
		void _handleIncomingConnection(ListeningSocket *listeningSocket);
		void _setupSource(Connection *conn) throw(ChildProcessNeededException);
		void _readFromSocket(Connection *conn) throw(ChildProcessNeededException);
		void _writeToSocket(Connection &conn);
		void _readFromSource(Connection &conn);
		void _writeToSource(Connection &conn);
		void _updateEvents(int action, int events, Connection *connection, int fd);
		void _updateEpoll(int action, int events, Connection *connection, int fd);
		ListeningSocket *_findListeningSocket(int fd);
		void _runNonPollableFds();

		void cleanup();
		void removeConnection(Connection *conn);
		void cleanInvalidatedConnections();
		void cleanIdleConnections();
		void cleanConnection(Connection *conn);
};
