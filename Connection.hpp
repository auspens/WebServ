/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Connection.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wouter <wouter@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 16:34:24 by auspensk          #+#    #+#             */
/*   Updated: 2025/04/26 16:49:54 by wouter           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "Request.hpp"
#include "SystemCallsUtilities.hpp"
#include "Socket.hpp"



class Connection{
	public:
		Connection(int fd);
		Connection(int fd, struct addrinfo *addrinfo);
		~Connection();

		int		getSourceFd() const;
		int		getSocketFd() const;
		bool	requestReady() const;
		void	readFromSocket(int buffer_size);
		void	writeToSocket(int buffer_size);
		void	prepSource();

	private:
		Connection(const Connection &src);
		Connection &operator=(const Connection &other);
		Connection();

		Socket	_socket;
		Request	_request;
};
