/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: auspensk <auspensk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 13:53:34 by auspensk          #+#    #+#             */
/*   Updated: 2025/04/23 17:49:32 by auspensk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "ListeningSocket.hpp"
#include "Connection.hpp"
#include <map>
#include <vector>


class Server : public SystemCallsUtilities{
	private:
		ListeningSocket _listeningSocket;
		int	_epollInstance;
		std::map<int, Connection>_connections;
		std::vector<struct epoll_event>_events;

		Server(Server const &src);
		Server &operator=(Server const &other);
	public:
		Server(std::string const &port, std::string const &host);
		Server();
		~Server();

		void openToConnections();
		void runEpollLoop();

};
