/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Connection.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: auspensk <auspensk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 16:34:24 by auspensk          #+#    #+#             */
/*   Updated: 2025/06/21 17:20:13 by auspensk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Config.hpp"
#include "Source/SourceFactory.hpp"
#include "ChildProcessNeededException.hpp"
#include "Logger.hpp"
#include "RequestParser.hpp"
#include "Response.hpp"
#include "Socket.hpp"
#include "SystemCallsUtilities.hpp"
#include <algorithm>
#include <ctime>
#include <string>
#include <sstream>
#include <vector>

class Connection{
	public:
		Connection(int fd, int serverPort);
		Connection(int fd, int serverPort, struct addrinfo *addrinfo);
		~Connection();

		int					getSourceFd() const;
		int					getSocketFd() const;
		HttpRequest			getRequest() const;
		Source				*getSource() const;
		const std::string	&getTarget() const;
		std::string			getRequestBody()const;
		time_t				getLastActiveTime() const;
		int					isInvalidated() const;

		void				setLastActiveTime(time_t time);
		void				setResponse();
		void				invalidate();

		bool				requestReady() const;
		bool				doneReadingSource()const;

		void				setupSource(const Config &config) throw(SourceAndRequestException, ChildProcessNeededException);
		void				setupErrorPageSource(const Config &config, int code)throw();

		void				readFromSocket(size_t bufferSize, const Config *config);
		void				writeToSocket();
		void				resetParser();


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
		time_t				_lastActiveTime;
};
