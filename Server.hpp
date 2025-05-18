/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wpepping <wpepping@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 13:53:34 by auspensk          #+#    #+#             */
/*   Updated: 2025/05/18 18:53:23 by wpepping         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <algorithm>
#include <map>
#include <vector>
#include "Connection.hpp"
#include "ListeningSocket.hpp"
#include "Config.hpp"

#define INFINITE_TIMEOUT -1

class Server {
	public:
		Server(const Config &config);
		~Server();

		void init();

	private:
		int									_epollInstance;
		const Config						*_config;
		std::map<int, ListeningSocket *>	_listeningSockets;
		std::vector<Connection*>			_connections;

		Server();
		Server(Server const &src);
		Server &operator=(Server const &other);

		void _listen();
		void _runEpollLoop();
		void _handleSocketEvent(struct epoll_event &event);
		void _handleIncomingConnection(ListeningSocket *listeningSocket);
		void _readFromSocket(Connection *conn);
		void _writeToSocket(Connection &conn);
		void _readFromSource(Connection &conn);
		void _updateEpoll(int action, int events, Connection *connection, int fd);
		ListeningSocket *_findListeningSocket(int fd);
};
