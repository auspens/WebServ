/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ShutDownSource.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: auspensk <auspensk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/06 17:46:12 by wouter            #+#    #+#             */
/*   Updated: 2025/07/16 14:59:14 by auspensk         ###   ########.fr       */
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

	_doneReading = true;
	setHeader();
}

void ShutDownSource::setHeader() {
	std::string response;

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

ShutDownSource::ShutDownSource(const ShutDownSource &other):Source(other){}
ShutDownSource &ShutDownSource::operator=(const ShutDownSource &other){
	if (this != &other){
		Source::operator=(other);
	}
	return *this;
}
