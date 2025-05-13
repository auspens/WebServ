/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Source.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: auspensk <auspensk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 09:54:14 by auspensk          #+#    #+#             */
/*   Updated: 2025/05/13 17:40:08 by auspensk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <exception>
#include <string>
#include "../ConfigClasses/ServerConfig.hpp"
#include <fstream>
#include <unistd.h>
#include <sys/stat.h>

class StaticFileSource;

enum SourceType {STATIC, REDIRECT, CGI};

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

	protected:
		int					_code;
		int					_fd;
		int					_size;
		SourceType			_type;
		ServerConfig		_serverConfig;
		Location			_location;
		std::string			_target;
		std::string			_mime;
		std::vector<char>	_body;

		Source(const std::string &target, const ServerConfig &serverConfig);

	private:
		static const Location &defineLocation(const std::string &target, const ServerConfig &serverConfig);
};
