/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ListeningSocket.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wpepping <wpepping@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 16:42:21 by auspensk          #+#    #+#             */
/*   Updated: 2025/05/18 18:08:22 by wpepping         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "Socket.hpp"
#include "WebServUtils.hpp"

#define BACKLOG 15

class ListeningSocket : public Socket{
	public:
		ListeningSocket(int port);
		~ListeningSocket();

		void	init();
		void	startListening();

	private:
		ListeningSocket();

		struct addrinfo	_hints;
		int 			_listeningPort;
};
