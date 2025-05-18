/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Connection.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wpepping <wpepping@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 16:34:24 by auspensk          #+#    #+#             */
/*   Updated: 2025/05/18 17:42:44 by wpepping         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Config.hpp"
#include "SystemCallsUtilities.hpp"
#include "Socket.hpp"
#include "StaticFileSource.hpp"
#include "RequestParser.hpp"
#include "Response.hpp"
#include <sstream>
#include <vector>

class Connection{
	public:
		Connection(int fd);
		Connection(int fd, struct addrinfo *addrinfo);
		~Connection();

		int					getSourceFd() const;
		int					getSocketFd() const;
		bool				requestReady() const;
		void				readFromSocket();
		void				writeToSocket();
		void				setResponse();
		void				resetParser();
		const std::string&	getTarget() const;
		Source				*getSource() const;
		void				setupSource(const Config &config) throw(Source::SourceException);
		void				sendHeader();
		void				sendFromSource();

	private:
		Connection(const Connection &src);
		Connection &operator=(const Connection &other);
		Connection();

		const	ServerConfig *_findServerConfig(
			int port,
			const std::string host,
			const Config &config
		) const;
		bool	_matchServerName(std::string host, std::string serverName) const;

		Socket			_socket;
		RequestParser	_parser;
		HttpRequest		_request;
		Response		*_response;
		Source			*_source;
		ServerConfig	*_serverConfig;
		int				_incPort;
};
