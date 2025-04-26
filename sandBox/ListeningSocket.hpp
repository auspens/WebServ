/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ListeningSocket.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wouter <wouter@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 16:42:21 by auspensk          #+#    #+#             */
/*   Updated: 2025/04/26 17:39:06 by wouter           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "Socket.hpp"
#include "WebServUtils.hpp"

#define BACKLOG 15

class ListeningSocket : public Socket{
	public:
		ListeningSocket(int port, std::string const &host);
		~ListeningSocket();

		void				bindSocket();
		void				startListening();
		//struct epoll_event	*getEpollevent();

	private:
		struct addrinfo _hints;
		ListeningSocket();
};
