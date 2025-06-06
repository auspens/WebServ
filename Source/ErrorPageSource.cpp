/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ErrorPageSource.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: auspensk <auspensk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/06 15:55:07 by auspensk          #+#    #+#             */
/*   Updated: 2025/06/06 16:02:53 by auspensk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ErrorPageSource.hpp"

ErrorPageSource::ErrorPageSource
		(const ServerConfig &serverConfig,
		Location const *location,
		HttpRequest req, int code)
		:	StaticFileSource(serverConfig, location, req){
	_code = code;
	getErrorPage(code);
}
		ErrorPageSource(const ErrorPageSource &src);
		ErrorPageSource &operator=(const ErrorPageSource &other);
		~ErrorPageSource();
