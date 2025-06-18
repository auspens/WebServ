/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ErrorPageSource.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wpepping <wpepping@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/06 15:37:02 by auspensk          #+#    #+#             */
/*   Updated: 2025/06/17 19:04:01 by wpepping         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Logger.hpp"
#include "StaticFileSource.hpp"

class ErrorPageSource : public StaticFileSource{
	public:
		ErrorPageSource(const ServerConfig &serverConfig, Location const *location, HttpRequest req, int code);
		ErrorPageSource(const ErrorPageSource &src);
		ErrorPageSource &operator=(const ErrorPageSource &other);
		~ErrorPageSource();

		void getErrorPage(int index);
};

