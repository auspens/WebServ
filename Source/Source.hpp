/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Source.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: auspensk <auspensk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 09:54:14 by auspensk          #+#    #+#             */
/*   Updated: 2025/05/06 18:13:27 by auspensk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <exception>
#include <string>
#include "../ConfigClasses/ServerConfig.hpp"
#include <fstream>
#include "RedirectSource.hpp"
#include "StaticFileSource.hpp"
#include <unistd.h>

class StaticFileSource;

enum SourceType {STATIC, REDIRECT, CGI};

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

		static Source *				getNewSource(const std::string &target, const ServerConfig &serverConfig);

	protected:
		int					_code;
		int					_fd;
		SourceType			_type;
		ServerConfig		_serverConfig;
		Location			_location;
		std::string			_target;
		std::string			_mime;
		std::vector<char>	_bytesRead;
		Source(const std::string &target, const ServerConfig &serverConfig);

	private:
		static const Location &defineLocation(const std::string &target, const ServerConfig &serverConfig);
};
