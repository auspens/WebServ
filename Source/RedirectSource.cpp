/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RedirectSource.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wpepping <wpepping@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 16:39:22 by auspensk          #+#    #+#             */
/*   Updated: 2025/06/19 17:36:57 by wpepping         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "RedirectSource.hpp"

RedirectSource::RedirectSource(const ServerConfig &serverConfig, const Location &location, HttpRequest req)
		: Source(serverConfig, &location, req)
{
	Logger::debug() << "Redirect source constructor"<<std::endl;
	_type = REDIRECT;
	_code = location.getRedirectCode();
	_target = req.path;
	_doneReading = true;
}

RedirectSource::~RedirectSource() { }

void RedirectSource::readSource() { }
