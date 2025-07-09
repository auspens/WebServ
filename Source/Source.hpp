/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Source.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wpepping <wpepping@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 09:54:14 by auspensk          #+#    #+#             */
/*   Updated: 2025/07/09 19:07:07 by wpepping         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <exception>
#include <fstream>
#include <string>
#include "ChildProcessNeededException.hpp"
#include "ShutDownRequestException.hpp"
#include "SourceAndRequestException.hpp"
#include "Config.hpp"
#include "Logger.hpp"
#include "HttpRequest.hpp"
#include "ServerConfig.hpp"
#include <unistd.h>
#include <sys/stat.h>

enum SourceType {
	STATIC,
	REDIRECT,
	CGI,
	UPLOAD,
	DELETE
};

struct HTTPStatusCode{
	std::string code;
	std::string message;
	std::string description;
};

#define STATUS_CODES_JSON "StatusCodes.json"
#define MIME_TYPES_JSON "FileExtesionsToMime.json"

class Source {
	public:
		virtual 					~Source();
		virtual void 				readSource() = 0;
		virtual void 				writeSource();
		virtual void				init() throw(SourceAndRequestException, ChildProcessNeededException, ShutDownRequestException);

		int 						getCode()const;
		std::string 				getMime()const;
		std::vector<char> const &	getBytesRead()const;
		int 						getFd()const;
		int 						getWriteFd()const;
		int							getSize()const;
		SourceType					getType()const;
		std::string					getRedirectLocation()const;
		virtual char				*readFromBuffer();
		bool						isPollableRead();
		bool						isPollableWrite();
		bool						isWriteWhenComplete();
		void						setHeader(std::string header);

		size_t										_bytesToSend;
		int											_offset;
		bool										_doneReading;
		bool										_doneWriting;
		static std::map<std::string, std::string>	_mimeTypes;
		static std::map<int, HTTPStatusCode>		_statusCodes;

	protected:
		int					_fd;
		int					_writeFd;
		int					_size;
		SourceType			_type;
		const ServerConfig	&_serverConfig;
		const Location		*_location;
		std::string			_target;
		std::string			_mime;
		HttpRequest			_request;
		int					_code;
		bool				_pollableRead;
		bool				_pollableWrite;
		bool				_writeWhenComplete;
		std::vector<char>	_body; //where we are writing

		void _checkAccess(std::string &target);
		bool _checkExists(std::string &target);

		Source(const ServerConfig &serverConfig, const Location *location, HttpRequest &req)
			throw(SourceAndRequestException);
		Source(const ServerConfig &serverConfig, const Location *location, HttpRequest &req, int code)
			throw(SourceAndRequestException);
		Source(const Source &src);
		Source &operator=(const Source &other);

	private:
		bool			_safePath(const std::string &path) const;
};
