/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ErrorPageSource.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wpepping <wpepping@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/06 15:55:07 by auspensk          #+#    #+#             */
/*   Updated: 2025/07/11 17:08:45 by wpepping         ###   ########.fr       */
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

void ErrorPageSource::init() throw(SourceAndRequestException) {
	_body.clear();
	_mime = "text/html";
	getErrorPage(_code);
	_offset = 0;
	_bytesToSend = _body.size();
	_doneReading = true;
	_doneWriting = true;
	_type = STATIC;
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
