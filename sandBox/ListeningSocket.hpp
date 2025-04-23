/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ListeningSocket.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: auspensk <auspensk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 16:42:21 by auspensk          #+#    #+#             */
/*   Updated: 2025/04/23 17:11:19 by auspensk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "Socket.hpp"
#define BACKLOG 15

class ListeningSocket : public Socket{
	public:
		ListeningSocket(std::string const &port, std::string const &host);
		void bindSocket();
		void startListening();
		struct epoll_event *getEpollevent();
		~ListeningSocket();
		
	private:
		struct addrinfo _hints;
		struct epoll_event *_epollEvent;
		ListeningSocket();
};
