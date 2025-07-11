/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RedirectSource.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wpepping <wpepping@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 16:39:22 by auspensk          #+#    #+#             */
/*   Updated: 2025/07/11 17:08:27 by wpepping         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "RedirectSource.hpp"

RedirectSource::RedirectSource(const ServerConfig &serverConfig, const Location &location, HttpRequest &req)
		: Source(serverConfig, &location, req)
{
	Logger::debug() << "Creating redirect source"<<std::endl;

}
void RedirectSource::init() throw(SourceAndRequestException) {
	_type = REDIRECT;
	_code = _location->getRedirectCode();
	_target = _request.path;
	_doneReading = true;
	_body.clear();
	_size = 0;
}

RedirectSource::~RedirectSource() { }

void RedirectSource::readSource() { }
