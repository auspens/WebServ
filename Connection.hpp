/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Connection.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wpepping <wpepping@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 16:34:24 by auspensk          #+#    #+#             */
<<<<<<< HEAD
/*   Updated: 2025/06/13 16:01:17 by auspensk         ###   ########.fr       */
=======
/*   Updated: 2025/06/17 19:27:46 by wpepping         ###   ########.fr       */
>>>>>>> e6b747404726033e3cd51df4848054da236c56e1
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Config.hpp"
<<<<<<< HEAD
#include "Source/SourceFactory.hpp"
=======
#include "ChildProcessNeededException.hpp"
#include "Logger.hpp"
>>>>>>> e6b747404726033e3cd51df4848054da236c56e1
#include "RequestParser.hpp"
#include "Response.hpp"
#include "Socket.hpp"
#include "SystemCallsUtilities.hpp"
#include <algorithm>
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
<<<<<<< HEAD
		void				readFromSocket(const Config &config);
=======
		void				readFromSocket(size_t bufferSize);
>>>>>>> e6b747404726033e3cd51df4848054da236c56e1
		void				writeToSocket();
		void				setResponse();
		void				resetParser();
		const std::string&	getTarget() const;
		Source				*getSource() const;
		void				setupSource(const Config &config) throw(SourceAndRequestException, ChildProcessNeededException);
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
