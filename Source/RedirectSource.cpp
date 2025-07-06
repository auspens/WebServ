/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RedirectSource.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wouter <wouter@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 16:39:22 by auspensk          #+#    #+#             */
/*   Updated: 2025/07/06 17:45:25 by wouter           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "RedirectSource.hpp"

RedirectSource::RedirectSource(const ServerConfig &serverConfig, const Location &location, HttpRequest &req)
		: Source(serverConfig, &location, req)
{
	Logger::debug() << "Creating redirect source"<<std::endl;
	_type = REDIRECT;
	_code = location.getRedirectCode();
	_target = req.path;
	_doneReading = true;
}

RedirectSource::~RedirectSource() { }

void RedirectSource::readSource() { }
