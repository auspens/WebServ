/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Source.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: auspensk <auspensk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 09:54:14 by auspensk          #+#    #+#             */
/*   Updated: 2025/05/05 15:23:01 by auspensk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <exception>
#include <string>
#include "../ConfigClasses/ServerConfig.hpp"
#include <fstream>

class Source {
	public:
		class SourceException : std::exception{
				std::string _error;
			public:
				SourceException(std::string error) throw();
				~SourceException()throw();
		};
		virtual void read() = 0;
		virtual ~Source();
		int getCode()const;
		std::string getMime()const;
		int getSize()const;
		std::vector<char> const &getBytesRead()const;

	protected:
		int					_code;
		ServerConfig		_serverConfig;
		Location			_location;
		std::string			_target;
		std::string			_mime;
		int					_size;
		std::vector<char>	_bytesRead;
		Source(const std::string &target, const ServerConfig &serverConfig);
	private:
		const Location &defineLocation(const std::string &target, const ServerConfig &serverConfig);
};
