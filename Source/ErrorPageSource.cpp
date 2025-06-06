/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ErrorPageSource.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: auspensk <auspensk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/06 15:55:07 by auspensk          #+#    #+#             */
/*   Updated: 2025/06/06 18:11:44 by auspensk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ErrorPageSource.hpp"

ErrorPageSource::ErrorPageSource
		(const ServerConfig &serverConfig,
		Location const *location,
		HttpRequest req, int code)
		:	StaticFileSource(serverConfig, location, req, code){
	_code = code;
	getErrorPage(code);
	//read error page right away to avoid looping in the reading error. If there is error in reading error page, generate one.
	_body.clear();
	_body.resize(_size);
	_offset = 0;
	ssize_t readSize = read(_fd, _body.data(), _size);
	if (readSize < 0)
		generatePage(code);
	else{
		_body.resize(readSize);
		_bytesToSend =readSize;
	}
	_doneReading = true;
}

void ErrorPageSource::getErrorPage(int code){
	// doesn't handle cases when error directive uses external URLs, like: error_page 404 https://example.com/notfound.html
	// I'm not sure if we need to include this feature. Doesn't say anything in the subject
	if (Config::getErrorPages(_serverConfig, _location).find(code) != Config::getErrorPages(_serverConfig, _location).end())
		_target = Config::getErrorPages(_serverConfig, _location).find(code)->second;
	else
		generatePage(code);
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
