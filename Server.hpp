/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wpepping <wpepping@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 13:53:34 by auspensk          #+#    #+#             */
/*   Updated: 2025/05/16 17:16:28 by wpepping         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <vector>
#include "Connection.hpp"
#include "ListeningSocket.hpp"
#include "ServerConfig.hpp"
#include "StaticFileSource.hpp"

#define INFINITE_TIMEOUT -1

class Server {
	private:
		int								_epollInstance;
		const ServerConfig				&_config;
		std::vector<ListeningSocket *>	_listeningSockets;
		std::vector<Connection*>		_connections;

		Server(Server const &src);
		Server &operator=(Server const &other);

		void _listen();
		void _runEpollLoop();
		void _handleSocketEvent(struct epoll_event &event);
		void _handleIncomingConnection(ListeningSocket &listeningSocket);
		void _readFromSocket(Connection *conn);
		void _writeToSocket(Connection &conn);
		void _readFromSource(Connection &conn);
		void _updateEpoll(int action, int events, Connection *connection, int fd);

	public:
		Server();
		Server(const ServerConfig &config);
		~Server();

		void init();
};
