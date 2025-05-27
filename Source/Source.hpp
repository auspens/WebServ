/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Source.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: auspensk <auspensk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 09:54:14 by auspensk          #+#    #+#             */
/*   Updated: 2025/05/27 10:43:13 by auspensk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <exception>
#include <string>
#include "ServerConfig.hpp"
#include <fstream>
#include <unistd.h>
#include <sys/stat.h>

class StaticFileSource;

enum SourceType {STATIC, REDIRECT, CGI};
struct HTTPStatusCode{
	std::string code;
	std::string message;
	std::string description;
};

#define STATUS_CODES_JSON "StatusCodes.json"
#define MIME_TYPES_JSON "FileExtesionsToMime.json"


class Source {
	public:
		class SourceException : std::exception{
				std::string _error;
			public:
				SourceException(std::string error) throw();
				const char *what() const throw();
				~SourceException()throw();
		};
		virtual 					~Source();
		virtual void 				readSource() = 0;
		virtual char *				getBufferToSend() = 0;//returns a buffer that can be sent through socket

		int 						getCode()const;
		std::string 				getMime()const;
		std::vector<char> const &	getBytesRead()const;
		int 						getFd()const;
		int							getSize()const;
		SourceType					getType()const;
		std::string					getLocation()const;

		static Source *				getNewSource(const std::string &target, const ServerConfig &serverConfig);


		int							_bytesToSend;
		int							_offset;
		static std::map<std::string, std::string> _mimeTypes;
		static std::map<int, HTTPStatusCode> _statusCodes;

	protected:
		int					_code;
		int					_fd;
		int					_size;
		SourceType			_type;
		const ServerConfig	&_serverConfig;
		const Location			*_location;
		std::string			_target;
		std::string			_mime;
		std::vector<char>	_body;

		Source(const std::string &target, const ServerConfig &serverConfig);
		char *readFromBuffer();

	private:
		static const Location *defineLocation(const std::string &target, const ServerConfig &serverConfig);

};
