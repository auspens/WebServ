/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wouter <wouter@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 13:53:34 by auspensk          #+#    #+#             */
/*   Updated: 2025/07/03 19:45:10 by wouter           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <algorithm>
#include <list>
#include <map>
#include <vector>
#include "ChildProcessNeededException.hpp"
#include "Config.hpp"
#include "Connection.hpp"
#include "Constants.hpp"
#include "EventInfo.hpp"
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
		std::list<EventInfo *>				_nonPollableReadFds;
		std::list<EventInfo *>				_nonPollableWriteFds;

		Server();
		Server(Server const &src);
		Server &operator=(Server const &other);

		void			_runEpollLoop() throw(ChildProcessNeededException);
		void			_handleEpollEvent(u_int32_t event, EventInfo *eventInfo) throw(ChildProcessNeededException);
		void			_handleIncomingConnection(ListeningSocket *listeningSocket);
		void			_handleSourceEvent(u_int32_t events, EventInfo *eventInfo) throw(ChildProcessNeededException);
		void			_handleSocketEvent(u_int32_t events, EventInfo *eventInfo);
		void			_setupSource(Connection *conn) throw(ChildProcessNeededException, SourceAndRequestException);
		void			_readFromSocket(EventInfo &eventInfo) throw(ChildProcessNeededException);
		void			_writeToSocket(EventInfo &eventInfo);
		void			_readFromSource(EventInfo &eventInfo);
		void			_writeToSource(EventInfo &eventInfo);
		void			_iterateNonPollables(std::list<EventInfo *> fds, u_int32_t eventType);
		void			_updateEvents(int action, u_int32_t events, EventInfo *eventInfo, int fd);
		void			_updateEpoll(int action, u_int32_t events, EventInfo *eventInfo, int fd);
		void			_updateNonPollables(int action, u_int32_t events, EventInfo *eventInfo);
		ListeningSocket	*_findListeningSocket(int fd);
		void			_cleanup();
		void			_removeConnection(Connection *conn);
		void			_cleanInvalidatedConnections();
		void			_cleanIdleConnections();
		void			_cleanConnection(Connection *conn);
};
