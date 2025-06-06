/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RedirectSource.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: auspensk <auspensk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 16:39:22 by auspensk          #+#    #+#             */
/*   Updated: 2025/06/06 13:55:06 by auspensk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "RedirectSource.hpp"

RedirectSource::RedirectSource(const ServerConfig &serverConfig, const Location &location, HttpRequest req)
		: Source(serverConfig, &location, req)
{
	std::cout <<"Redirect source constructor"<<std::endl;
	_type = REDIRECT;
	_code = location.getRedirectCode();
	_target = req.path;
	_doneReading = true;
}

RedirectSource::~RedirectSource()
{
}

void RedirectSource::readSource(){}
char *RedirectSource::getBufferToSend(){
	return NULL;
}
