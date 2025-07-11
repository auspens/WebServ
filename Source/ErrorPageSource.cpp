/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ErrorPageSource.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: auspensk <auspensk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/06 15:55:07 by auspensk          #+#    #+#             */
/*   Updated: 2025/07/11 16:26:13 by auspensk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ErrorPageSource.hpp"


ErrorPageSource::ErrorPageSource
		(const ServerConfig &serverConfig,
		Location const *location,
		HttpRequest &req, int code)
		: StaticFileSource(serverConfig, location, req){
	_code = code;
}

void ErrorPageSource::getErrorPage(int code){
	if (Config::getErrorPages(_serverConfig, _location).find(code) != Config::getErrorPages(_serverConfig, _location).end()){
		_target = Config::getErrorPages(_serverConfig, _location).find(code)->second;
		_fd = open(_target.c_str(), O_RDONLY);
		struct stat st;
		stat(_target.c_str(), &st);
		_size = st.st_size;
		_body.resize(_size);
		ssize_t readSize = read(_fd, _body.data(), _size);
		if (readSize < 0)
			return generatePage(code);
		defineMimeType();
	}
	else
		generatePage(code);
}

void ErrorPageSource::init(const ServerConfig &serverConfig, const Location *location, HttpRequest &req) throw(SourceAndRequestException){
	(void)serverConfig;
	(void)location;
	(void)req;
	_body.clear();
	_mime = "text/html";
	getErrorPage(_code);
	_offset = 0;
	_bytesToSend = _body.size();
	_doneReading = true;
	_doneWriting = true;
	setHeader();
}

ErrorPageSource::ErrorPageSource(const ErrorPageSource &src):StaticFileSource(src){
}

ErrorPageSource &ErrorPageSource::operator=(const ErrorPageSource &other){
	if (this != &other){
		StaticFileSource::operator=(other);
	}
	return *this;
}

ErrorPageSource::~ErrorPageSource(){}

void ErrorPageSource::setHeader(){
	std::string header;
	header += std::string("HTTP/1.1 ") + _statusCodes[_code].code + " " + _statusCodes[_code].description + "\r\n";
	header += "Content-Type: " + _mime + "\r\n";
	header += "Content-Length: " + WebServUtils::num_to_str(_size) + "\r\n\r\n";
	Logger::debug()<< "At setHeader" << std::endl;
	Logger::debug() << "Body: " << std::string(_body.begin(), _body.end())<< " bytesTosend: "<< _bytesToSend<<std::endl;
	Logger::debug()<< "Header: " << header << "header length: "<< header.length()<<std::endl;
	_body.insert(_body.begin(), header.begin(), header.end());
	_bytesToSend += header.length();
	Logger::debug() << "Bytes to send: " << _bytesToSend << std::endl;
}
