/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RedirectSource.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: auspensk <auspensk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 16:39:22 by auspensk          #+#    #+#             */
/*   Updated: 2025/07/16 14:56:49 by auspensk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "RedirectSource.hpp"

RedirectSource::RedirectSource(const ServerConfig &serverConfig, const Location &location, HttpRequest &req)
		: Source(serverConfig, &location, req)
{
	Logger::debug() << "Creating redirect source"<<std::endl;

}

RedirectSource::RedirectSource(const RedirectSource &other):Source(other){
	_code = other._code;
}
RedirectSource &RedirectSource::operator=(const RedirectSource &other){
	if (this != &other){
		Source::operator=(other);
		_code = other._code;
	}
	return *this;
}

void RedirectSource::init() throw(SourceAndRequestException) {
	_code = _location->getRedirectCode();
	_target = _request.path;
	_doneReading = true;
	_doneWriting = true;
	_body.clear();
	_size = 0;
	setHeader();
}

std::string RedirectSource::getRedirectLocation() const {
	if (!_location->isRedirect())
		return std::string("");
	return _location->getRedirectPath();
}

void RedirectSource::setHeader(){
	std::string header;
	header += std::string(PROTOCOL) + " " + WebServUtils::num_to_str(_code) + " " + StatusCodesStorage::getStatusCodes().find(_code)->second.message + "\r\n";
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
