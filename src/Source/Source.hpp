/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Source.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wpepping <wpepping@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 09:54:14 by auspensk          #+#    #+#             */
/*   Updated: 2025/07/16 15:59:23 by wpepping         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <exception>
#include <fstream>
#include <string>
#include "IsChildProcessException.hpp"
#include "SourceAndRequestException.hpp"
#include "StatusCodesStorage.hpp"
#include "Config.hpp"
#include "Logger.hpp"
#include "HttpRequest.hpp"
#include "ServerConfig.hpp"
#include <unistd.h>

class Source {
	public:
		virtual 					~Source();
		virtual void 				readSource() = 0;
		virtual void 				writeSource() throw(SourceAndRequestException);
		virtual void				init() throw(SourceAndRequestException);
		virtual bool				checkTimeout(int timeout) const;
		virtual void				setHeader();

		std::string 				getMime() const;
		std::vector<char> const &	getBytesRead() const;
		int 						getFd() const;
		int 						getWriteFd() const;
		int							getSize() const;
		bool						isPollableRead() const;
		bool						isPollableWrite() const;
		bool						isWriteWhenComplete() const;

		virtual char				*readFromBuffer();
		void						bytesSent(int bytes);
		void						finalizeWrite();

		size_t										_bytesToSend;
		int											_offset;
		bool										_doneReading;
		bool										_doneWriting;
		static std::map<std::string, std::string>	_mimeTypes;

	protected:
		int					_fd;
		int					_writeFd;
		size_t				_size;
		const ServerConfig	&_serverConfig;
		const Location		*_location;
		std::string			_target;
		std::string			_mime;
		HttpRequest			_request;
		bool				_pollableRead;
		bool				_pollableWrite;
		bool				_writeWhenComplete;
		std::vector<char>	_body; //where we are writing

		void _checkAccess(std::string &target);
		bool _checkExists(std::string &target);

		Source(const ServerConfig &serverConfig, const Location *location, HttpRequest &req)
			throw(SourceAndRequestException);
		Source(const Source &src);
		Source &operator=(const Source &other);

	private:
		bool			_safePath(const std::string &path) const;
};
