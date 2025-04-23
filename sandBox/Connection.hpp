/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Connection.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: auspensk <auspensk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 16:34:24 by auspensk          #+#    #+#             */
/*   Updated: 2025/04/23 16:59:58 by auspensk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "SystemCallsUtilities.hpp"
#include "Socket.hpp"

class Connection{
	private:
		Socket _socket;
		struct epoll_event *_epollEvent;
		Connection(const Connection &src);
		Connection &operator=(const Connection &other);
		Connection();
	public:
		Connection(int fd, struct addrinfo *addrinfo);
		~Connection();
		struct epoll_event *getEpollevent();

};
