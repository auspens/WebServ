/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Connection.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wpepping <wpepping@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 16:34:24 by auspensk          #+#    #+#             */
/*   Updated: 2025/07/16 17:20:17 by wpepping         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Config.hpp"
#include "Source/SourceFactory.hpp"
#include "IsChildProcessException.hpp"
#include "EmptyRequestException.hpp"
#include "EventInfo.hpp"
#include "Logger.hpp"
#include "RequestParser.hpp"
#include "Socket.hpp"
#include "SystemCallsUtilities.hpp"
#include <algorithm>
#include <ctime>
#include <string>

class Connection{
	public:
		Connection(int fd, int serverPort, size_t bufferSize);
		~Connection();

		class SocketException : std::exception{
			public:
				virtual const char *what() const throw();
				SocketException(const std::string& msg);
				SocketException();
				virtual ~SocketException() throw();

			private:
				std::string message;
		};

		int					getSourceFd() const;
		int					getSocketFd() const;
		HttpRequest			getRequest() const;
		Source				*getSource() const;
		const std::string	&getTarget() const;
		std::string			getRequestBody() const;
		time_t				getLastActiveTime() const;
		EventInfo			*getSourceEventInfo() const;
		EventInfo			*getSocketEventInfo() const;
		bool				isInvalidated() const;
		bool				checkSocketTimeout(int timeout) const;

		void				invalidate();
		bool				requestReady() const;
		bool				doneReadingSource() const;
		bool				doneWritingSource() const;
		bool				doneWritingSocket() const;

		void				setupSource(bool &shutDownFlag) throw(SourceAndRequestException, IsChildProcessException);
		void				setupErrorPageSource(const Config &config, int code)throw();

		void				readFromSocket(size_t bufferSize, const Config *config)
								throw(SourceAndRequestException, EmptyRequestException, SocketException);
		void				writeToSocket() throw(SocketException);
		void				finishRequest();

	private:
		Connection(const Connection &src);
		Connection &operator=(const Connection &other);
		Connection();

		const ServerConfig *_findServerConfig(
			int port,
			const std::string host,
			const Config &config
		) const;

		const Location *_findLocation (
			const std::string &target,
			const ServerConfig &serverConfig
		);

		bool _matchServerName(std::string host, std::string serverName) const;
		void _setLastSocketActiveTime(time_t time);
		void _resetParser();

		Socket				_socket;
		RequestParser		_parser;
		HttpRequest			_request;
		Source				*_source;
		const ServerConfig	*_serverConfig;
		const Location		*_location;
		int					_serverPort;
		bool				_invalidated;
		time_t				_lastActiveTime;
		EventInfo			*_sourceEventInfo;
		EventInfo			*_socketEventInfo;
		std::vector<char>	_socketReadBuffer;
};
