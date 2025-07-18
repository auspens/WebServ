/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wpepping <wpepping@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 13:53:34 by auspensk          #+#    #+#             */
/*   Updated: 2025/07/17 17:14:20 by wpepping         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <algorithm>
#include <list>
#include <map>
#include <vector>
#include "Config.hpp"
#include "Connection.hpp"
#include "Constants.hpp"
#include "EventInfo.hpp"
#include "IsChildProcessException.hpp"
#include "ListeningSocket.hpp"
#include "Logger.hpp"

#define INFINITE_TIMEOUT -1

class Server {
	public:
		Server(const Config &config);
		~Server();

		void listen() throw(IsChildProcessException, ListeningSocket::SocketOpenFailedException);

		static int childProcessMonitorPipe[2];

	private:
		int									_epollInstance;
		time_t								_lastCleanup;
		const Config						*_config;
		bool								_shutDownFlag;
		std::map<int, ListeningSocket *>	_listeningSockets;
		std::vector<Connection *>			_connections;
		std::vector<Connection *>			_invalidatedConnections;
		std::list<EventInfo *>				_nonPollableReadFds;
		std::list<EventInfo *>				_nonPollableWriteFds;
		EventInfo							*_childMonitorEventInfo;

		Server();
		Server(Server const &src);
		Server &operator=(Server const &other);

		ListeningSocket *newListeningSocket(int port) throw(ListeningSocket::SocketOpenFailedException);

		void			_runEpollLoop() throw(IsChildProcessException);
		void			_handleEpollEvent(u_int32_t event, EventInfo *eventInfo) throw(IsChildProcessException);
		void			_handleIncomingConnection(ListeningSocket *listeningSocket);
		void			_handleSourceEvent(u_int32_t events, EventInfo *eventInfo) throw(IsChildProcessException);
		void			_handleSocketEvent(u_int32_t events, EventInfo *eventInfo);
		void			_handleChildProcessEvent();
		void			_setupSource(Connection *conn) throw(IsChildProcessException, SourceAndRequestException);
		void			_readFromSocket(EventInfo &eventInfo) throw(IsChildProcessException);
		void			_writeToSocket(EventInfo &eventInfo);
		void			_readFromSource(EventInfo &eventInfo);
		void			_writeToSource(EventInfo &eventInfo);
		void			_finishRequest(Connection *conn);
		void			_handleSourceError(Connection *conn, int errorCode);
		void			_iterateNonPollables(std::list<EventInfo *> fds, u_int32_t eventType);
		void			_updateEvents(int action, u_int32_t events, EventInfo *eventInfo, int fd);
		void			_updateEpoll(int action, u_int32_t events, EventInfo *eventInfo, int fd);
		void			_updateNonPollables(int action, u_int32_t events, EventInfo *eventInfo);
		ListeningSocket	*_findListeningSocket(int fd);
		void			_cleanup();
		void			_removeConnection(Connection *conn);
		void			_cleanInvalidatedConnections();
		void			_checkTimeouts();
		void			_cleanConnection(Connection *conn);
};
