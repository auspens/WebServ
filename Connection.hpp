/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Connection.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eleonora <eleonora@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 16:34:24 by auspensk          #+#    #+#             */
/*   Updated: 2025/05/03 22:23:50 by eleonora         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "SystemCallsUtilities.hpp"
#include "Socket.hpp"

#include "RequestParser.hpp"
#include "HttpResponse.hpp"

#include <sstream>

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
		void 	generateResponse();
		void	resetParser();


	private:
		Connection(const Connection &src);
		Connection &operator=(const Connection &other);
		Connection();

		Socket	_socket;
		RequestParser _parser;
		HttpRequest	_request;
		HttpResponse _response; 
};
