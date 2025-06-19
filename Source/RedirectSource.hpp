/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RedirectSource.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wpepping <wpepping@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 16:37:31 by auspensk          #+#    #+#             */
/*   Updated: 2025/06/19 17:45:04 by wpepping         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Logger.hpp"
#include "Source.hpp"

class RedirectSource : public Source
{
	public:
		RedirectSource(const ServerConfig &serverConfig, const Location &location, HttpRequest req);
		~RedirectSource();
		void readSource();
};



