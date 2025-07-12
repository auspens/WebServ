/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RedirectSource.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wouter <wouter@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 16:37:31 by auspensk          #+#    #+#             */
/*   Updated: 2025/07/12 21:34:22 by wouter           ###   ########.fr       */
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

		void		readSource();
		void		init() throw(SourceAndRequestException);
		void		setHeader();
		std::string	getRedirectLocation() const;

	private:
		int _code;
};



