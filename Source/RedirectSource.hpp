/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RedirectSource.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: auspensk <auspensk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 16:37:31 by auspensk          #+#    #+#             */
/*   Updated: 2025/07/11 13:47:11 by auspensk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Logger.hpp"
#include "Source.hpp"

class RedirectSource : public Source
{
	public:
		RedirectSource(const ServerConfig &serverConfig, const Location &location, HttpRequest &req);
		~RedirectSource();
		void readSource();
		void init(const ServerConfig &serverConfig, const Location *location, HttpRequest &req) throw(SourceAndRequestException);
};



