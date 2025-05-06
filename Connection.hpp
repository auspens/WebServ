/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Connection.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: auspensk <auspensk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 16:34:24 by auspensk          #+#    #+#             */
/*   Updated: 2025/04/30 16:40:46 by auspensk         ###   ########.fr       */
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
		const	std::string &getRequestTarget()const;

	private:
		Connection(const Connection &src);
		Connection &operator=(const Connection &other);
		Connection();

		Socket	_socket;
		Request	_request;
};
