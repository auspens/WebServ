/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Connection.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: auspensk <auspensk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 16:34:24 by auspensk          #+#    #+#             */
/*   Updated: 2025/06/10 11:33:10 by auspensk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Config.hpp"
#include "ChildProcessNeededException.hpp"
#include "RequestParser.hpp"
#include "Response.hpp"
#include "Socket.hpp"
#include "StaticFileSource.hpp"
#include "SystemCallsUtilities.hpp"
#include <sstream>
#include <vector>

class Connection{
	public:
		Connection(int fd, int serverPort);
		Connection(int fd, int serverPort, struct addrinfo *addrinfo);
		~Connection();

		int					getSourceFd() const;
		int					getSocketFd() const;
		bool				requestReady() const;
		HttpRequest			getRequest() const;
		void				readFromSocket();
		void				writeToSocket();
		void				setResponse();
		void				resetParser();
		const std::string&	getTarget() const;
		Source				*getSource() const;
		void				setupSource(const Config &config) throw(Source::SourceException, ChildProcessNeededException);
		void				setupErrorPageSource(const Config &config, int code)throw();
		void				sendHeader();
		void				sendFromSource();
		std::string			getRequestBody()const;
		bool				doneReadingSource()const;
		void				invalidate();
		int					isInvalidated() const;


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

		Socket				_socket;
		RequestParser		_parser;
		HttpRequest			_request;
		Response			*_response;
		Source				*_source;
		const ServerConfig	*_serverConfig;
		int					_serverPort;
		bool				_invalidated;
};
