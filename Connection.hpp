/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Connection.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: auspensk <auspensk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 16:34:24 by auspensk          #+#    #+#             */
/*   Updated: 2025/05/13 17:17:32 by auspensk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "SystemCallsUtilities.hpp"
#include "Socket.hpp"
#include "Source/StaticFileSource.hpp"

#include "RequestParser.hpp"
#include "Response.hpp"

#include <sstream>
#include <vector>

class Connection{
	public:
		Connection(int fd);
		Connection(int fd, struct addrinfo *addrinfo);
		~Connection();

		int				getSourceFd() const;
		int				getSocketFd() const;
		bool			requestReady() const;
		void			readFromSocket();
		void			writeToSocket();
		void			setResponse(const Source *source);
		void			resetParser();
		const std::string& getTarget() const;
		Source			*getSource() const;
		void			setSource(Source *source);
		void			sendHeader();
		void			sendFromSource();

	private:
		Connection(const Connection &src);
		Connection &operator=(const Connection &other);
		Connection();

		Socket			_socket;
		RequestParser	_parser;
		HttpRequest		_request;
		Response		*_response;
		Source			*_source;
};
