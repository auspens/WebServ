/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RedirectSource.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: auspensk <auspensk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 16:39:22 by auspensk          #+#    #+#             */
/*   Updated: 2025/07/11 13:58:24 by auspensk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "RedirectSource.hpp"

RedirectSource::RedirectSource(const ServerConfig &serverConfig, const Location &location, HttpRequest &req)
		: Source(serverConfig, &location, req)
{
	Logger::debug() << "Creating redirect source"<<std::endl;

}
void RedirectSource::init(const ServerConfig &serverConfig, const Location *location, HttpRequest &req) throw (SourceAndRequestException){
	(void)serverConfig;
	(void)location;
	(void)req;
	_type = REDIRECT;
	_code = location->getRedirectCode();
	_target = req.path;
	_doneReading = true;
	_body.clear();
	_size = 0;
}

RedirectSource::~RedirectSource() { }

void RedirectSource::readSource() { }
