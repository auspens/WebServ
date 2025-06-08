/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wouter <wouter@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 13:53:34 by auspensk          #+#    #+#             */
/*   Updated: 2025/06/08 16:05:17 by wouter           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <algorithm>
#include <map>
#include <vector>
#include "ChildProcessNeededException.hpp"
#include "Connection.hpp"
#include "ListeningSocket.hpp"
#include "Config.hpp"

#define INFINITE_TIMEOUT -1

class Server {
	public:
		Server(const Config &config);
		~Server();

		void listen() throw(ChildProcessNeededException);

	private:
		int									_epollInstance;
		const Config						*_config;
		std::map<int, ListeningSocket *>	_listeningSockets;
		std::vector<Connection*>			_connections;
		std::vector<Connection*>			_invalidatedConnections;

		Server();
		Server(Server const &src);
		Server &operator=(Server const &other);

		void _runEpollLoop() throw(ChildProcessNeededException);
		void _handleSocketEvent(struct epoll_event &event) throw(ChildProcessNeededException);
		void _handleIncomingConnection(ListeningSocket *listeningSocket);
		void _readFromSocket(Connection *conn) throw(ChildProcessNeededException);
		void _writeToSocket(Connection &conn);
		void _readFromSource(Connection &conn);
		int _updateEpoll(int action, int events, Connection *connection, int fd);
		ListeningSocket *_findListeningSocket(int fd);

		void cleanup();
		void configureCGI(Connection* conn);
		void removeConnection(Connection *conn);
		void cleanInvalidatedConnections();
};
