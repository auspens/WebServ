/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   DeleteSource.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wouter <wouter@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/06 17:46:12 by wouter            #+#    #+#             */
/*   Updated: 2025/07/06 19:14:26 by wouter           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "DeleteSource.hpp"

DeleteSource::DeleteSource(
	const ServerConfig &serverConfig,
	const Location &location,
	HttpRequest &req
) throw(SourceAndRequestException) :
	Source(serverConfig, &location, req)
{
	Logger::debug() << "Creating Delete source"<<std::endl;
	_type = DELETE;
	_doneReading = true;

	_checkAccess(_target);
	_deleteFile();
}

void DeleteSource::_deleteFile() throw(SourceAndRequestException) {
	if (access(_target.c_str(), W_OK))
		throw SourceAndRequestException("Forbidden", 403);
	if (remove(_target.c_str()) != 0)
		throw SourceAndRequestException("Could not delete file", 500);
	_setResponse();
}

void DeleteSource::_setResponse() {
	std::string response;

	response =
		"HTTP/1.1 200 OK\n"
		"Content-Type: application/json\n"
		"Content-Length: 41\n"
		"\n"
		"{\"message\": \"File deleted successfully\"}\n";

	_body.assign(response.begin(), response.end());
}

DeleteSource::~DeleteSource() { }

void DeleteSource::readSource() { }
