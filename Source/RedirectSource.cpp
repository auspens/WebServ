/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RedirectSource.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eusatiko <eusatiko@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 16:39:22 by auspensk          #+#    #+#             */
/*   Updated: 2025/05/28 14:25:11 by eusatiko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "RedirectSource.hpp"

RedirectSource::RedirectSource(const std::string &target, const ServerConfig &serverConfig, int code, HttpRequest req)
		:Source(target, serverConfig, req)
{
	std::cout <<"Redirect source constructor"<<std::endl;
	_type = REDIRECT;
	_code = code;
	_target = target;
}

RedirectSource::~RedirectSource()
{
}

void RedirectSource::readSource(){}
char *RedirectSource::getBufferToSend(){
	return NULL;
}
