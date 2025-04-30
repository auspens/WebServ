/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Source.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: auspensk <auspensk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 09:54:14 by auspensk          #+#    #+#             */
/*   Updated: 2025/04/30 16:31:01 by auspensk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <exception>
#include <string>
#include "../ConfigClasses/ServerConfig.hpp"

class Source {
	public:
		class SourceException : std::exception{
				std::string _error;
			public:
				SourceException(std::string error);
		};
		virtual void *read() = 0;
		virtual ~Source();
	protected:
		int 		_fd;
		int 		_code;
		Location 	_location;
		ServerConfig _serverConfig;
		std::string _target;
		Source(const std::string &target, const ServerConfig &serverConfig);
	private:
		const Location &defineLocation(const std::string &target, const ServerConfig &serverConfig);

};
