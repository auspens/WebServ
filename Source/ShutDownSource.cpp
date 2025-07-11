/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ShutDownSource.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wpepping <wpepping@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/06 17:46:12 by wouter            #+#    #+#             */
/*   Updated: 2025/07/11 17:09:05 by wpepping         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ShutDownSource.hpp"

ShutDownSource::ShutDownSource(
	const ServerConfig &serverConfig,
	const Location &location,
	HttpRequest &req
) :
	Source(serverConfig, &location, req)
{
	Logger::debug() << "Creating Shutdown source"<<std::endl;
}

void ShutDownSource::init() throw(SourceAndRequestException) {
	Source::init();

	_type = DELETE;
	_doneReading = true;
}

void ShutDownSource::setHeader(std::string header) {
	std::string response;

	(void)header;
	response =
		"HTTP/1.1 200 OK\n"
		"Content-Type: application/json\n"
		"Content-Length: 41\n"
		"\n"
		"{\"message\": \"Server shutting down\"}\n";

	_body.assign(response.begin(), response.end());
}

ShutDownSource::~ShutDownSource() { }

void ShutDownSource::readSource() { }
