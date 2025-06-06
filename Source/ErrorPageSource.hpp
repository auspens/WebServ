/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errorPageSource.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: auspensk <auspensk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/06 15:37:02 by auspensk          #+#    #+#             */
/*   Updated: 2025/06/06 15:48:52 by auspensk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "StaticFileSource.hpp"

class ErrorPageSource : StaticFileSource{
	public:
		ErrorPageSource(const ServerConfig &serverConfig, Location const *location, HttpRequest req, int code);
		ErrorPageSource(const ErrorPageSource &src);
		ErrorPageSource &operator=(const ErrorPageSource &other);
		~ErrorPageSource();
};

