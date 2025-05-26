/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RedirectSource.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: auspensk <auspensk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 16:39:22 by auspensk          #+#    #+#             */
/*   Updated: 2025/05/26 16:59:53 by auspensk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "RedirectSource.hpp"

RedirectSource::RedirectSource(const std::string &target, const ServerConfig &serverConfig, int code)
		:Source(target, serverConfig)
{
	std::cout <<"Redirect source constructor"<<std::endl;
	_type = REDIRECT;
	_code = code;
}

RedirectSource::~RedirectSource()
{
}

void RedirectSource::readSource(){}
char *RedirectSource::getBufferToSend(){
	return NULL;
}
