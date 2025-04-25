/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wouter <wouter@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 13:53:34 by auspensk          #+#    #+#             */
/*   Updated: 2025/04/25 17:29:56 by wouter           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "ListeningSocket.hpp"
#include "Connection.hpp"
#include "ServerConfig.hpp"
#include <map>
#include <vector>

#define INFINITE_TIMEOUT -1

class Server {
	private:
		ListeningSocket 			*_listeningSocket;
		int							_epollInstance;
		std::map<int, Connection>	_socketConnections;
		std::map<int, Connection>	_sourceConnections;

		Server(Server const &src);
		Server &operator=(Server const &other);

		void _listen();
		void _runEpollLoop();
		void _handleIncomingConnection(Connection conn);

	public:
		Server();
		Server(ServerConfig config);
		~Server();

		void init();
};
