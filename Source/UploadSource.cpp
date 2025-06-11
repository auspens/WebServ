/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   UploadSource.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: auspensk <auspensk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/29 16:03:49 by wpepping          #+#    #+#             */
/*   Updated: 2025/06/11 15:32:53 by auspensk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "UploadSource.hpp"

UploadSource::~UploadSource() { }

UploadSource::UploadSource(
	const ServerConfig &serverConfig,
	Location const &location,
	HttpRequest req
) : Source(serverConfig, &location, req) {
	
	_filename = req.path;
}
