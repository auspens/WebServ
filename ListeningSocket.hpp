/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ListeningSocket.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wpepping <wpepping@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 16:42:21 by auspensk          #+#    #+#             */
/*   Updated: 2025/05/15 19:39:46 by wpepping         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "Socket.hpp"
#include "WebServUtils.hpp"

#define BACKLOG 15

class ListeningSocket : public Socket{
	public:
		ListeningSocket(int port, std::string const *host);
		~ListeningSocket();

		void				bindSocket();
		void				startListening();
		//struct epoll_event	*getEpollevent();

	private:
		struct addrinfo _hints;
		ListeningSocket();
};
