/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ListeningSocket.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wpepping <wpepping@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 16:42:21 by auspensk          #+#    #+#             */
/*   Updated: 2025/07/17 16:47:52 by wpepping         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <netdb.h>
#include <vector>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdexcept>
#include <sys/epoll.h>
#include <unistd.h>
#include "EventInfo.hpp"
#include "Logger.hpp"
#include "Socket.hpp"
#include "WebServUtils.hpp"

#define BACKLOG 15

class ListeningSocket : public Socket{
	public:
		class SocketOpenFailedException : std::exception {
			public:
				virtual const char *what() const throw();
		};

		ListeningSocket(int port);
		~ListeningSocket();

		void		init();
		void		startListening() throw(SocketOpenFailedException);
		int			getPort();
		EventInfo	*getEventInfo() const;

	private:
		ListeningSocket();
		ListeningSocket(const ListeningSocket &other);
		ListeningSocket &operator=(const ListeningSocket &other);

		struct addrinfo	_hints;
		int 			_listeningPort;
		EventInfo		*_eventInfo;
};
