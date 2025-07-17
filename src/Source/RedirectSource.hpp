/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RedirectSource.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: auspensk <auspensk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 16:37:31 by auspensk          #+#    #+#             */
/*   Updated: 2025/07/16 14:54:31 by auspensk         ###   ########.fr       */
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
		RedirectSource(const RedirectSource &other);
		RedirectSource &operator=(const RedirectSource &other);

		void		readSource();
		void		init() throw(SourceAndRequestException);
		void		setHeader();
		std::string	getRedirectLocation() const;

	private:
		int _code;
};



