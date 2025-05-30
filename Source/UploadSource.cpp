/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   UploadSource.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wpepping <wpepping@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/29 16:03:49 by wpepping          #+#    #+#             */
/*   Updated: 2025/05/29 16:38:55 by wpepping         ###   ########.fr       */
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
