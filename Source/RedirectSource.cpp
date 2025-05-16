/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RedirectSource.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: auspensk <auspensk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 16:39:22 by auspensk          #+#    #+#             */
/*   Updated: 2025/05/16 16:27:13 by auspensk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "RedirectSource.hpp"

RedirectSource::RedirectSource(const std::string &target, const ServerConfig &serverConfig, int code)
		:Source(target, serverConfig)
{
	_type = REDIRECT;
	_code = code;
}

RedirectSource::~RedirectSource()
{
}

void RedirectSource::readSource(){}
char *RedirectSource::readFromSource(){
	return NULL;
}

