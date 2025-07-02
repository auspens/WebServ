/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ListeningSocket.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wpepping <wpepping@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 16:42:21 by auspensk          #+#    #+#             */
/*   Updated: 2025/07/02 18:47:31 by wpepping         ###   ########.fr       */
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
#include "Logger.hpp"
#include "Socket.hpp"
#include "WebServUtils.hpp"

#define BACKLOG 15

class ListeningSocket : public Socket{
	public:
		ListeningSocket(int port);
		~ListeningSocket();

		void	init();
		void	startListening();
		int		getPort();

	private:
		ListeningSocket();

		struct addrinfo	_hints;
		int 			_listeningPort;
};
