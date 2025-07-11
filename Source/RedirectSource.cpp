/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RedirectSource.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: auspensk <auspensk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 16:39:22 by auspensk          #+#    #+#             */
/*   Updated: 2025/07/11 16:10:01 by auspensk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "RedirectSource.hpp"

RedirectSource::RedirectSource(const ServerConfig &serverConfig, const Location &location, HttpRequest &req)
		: Source(serverConfig, &location, req)
{
	Logger::debug() << "Creating redirect source"<<std::endl;

}
void RedirectSource::init(const ServerConfig &serverConfig, const Location *location, HttpRequest &req) throw (SourceAndRequestException){
	(void)serverConfig;
	(void)location;
	(void)req;
	_code = location->getRedirectCode();
	_target = req.path;
	_doneReading = true;
	_doneWriting = true;
	_body.clear();
	_size = 0;
	setHeader();
}

void RedirectSource::setHeader(){
	std::string header;
	header += std::string(PROTOCOL) + " " + WebServUtils::num_to_str(_code) + " " + Source::_statusCodes.find(_code)->second.message + "\r\n";
	header += "Location: " + getRedirectLocation() + "\r\n";
	header += "Content-Length: 0\r\n\r\n";
	Logger::debug()<< "At setHeader" << std::endl;
	Logger::debug() << "Body: " << std::string(_body.begin(), _body.end())<< " bytesTosend: "<< _bytesToSend<<std::endl;
	Logger::debug()<< "Header: " << header << "header length: "<< header.length()<<std::endl;
	_body.insert(_body.begin(), header.begin(), header.end());
	_bytesToSend += header.length();
	Logger::debug() << "Bytes to send: " << _bytesToSend << std::endl;
}

RedirectSource::~RedirectSource() { }

void RedirectSource::readSource() { }
