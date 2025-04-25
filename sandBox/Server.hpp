/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wouter <wouter@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 13:53:34 by auspensk          #+#    #+#             */
/*   Updated: 2025/04/25 15:00:28 by wouter           ###   ########.fr       */
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
		std::map<int, Connection> _socketConnections;
		std::map<int, Connection> _sourceConnections;

		Server(Server const &src);
		Server &operator=(Server const &other);

	public:
		Server(int const &port, std::string const &host);
		Server();
		~Server();

		void openToConnections();
		void runEpollLoop();

};
