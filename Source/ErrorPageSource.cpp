/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ErrorPageSource.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wouter <wouter@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/06 15:55:07 by auspensk          #+#    #+#             */
/*   Updated: 2025/07/12 21:26:33 by wouter           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ErrorPageSource.hpp"


ErrorPageSource::ErrorPageSource
		(const ServerConfig &serverConfig,
		Location const *location,
		HttpRequest &req, int code)
		: StaticFileSource(
			serverConfig,
			location,
			req,
			Config::getErrorPages(serverConfig, location).find(code)->second
		) {
			_code = code;
}

void ErrorPageSource::getErrorPage(int code){
	if (Config::getErrorPages(_serverConfig, _location).find(code) != Config::getErrorPages(_serverConfig, _location).end()) {
		struct stat st;

		stat(_target.c_str(), &st);
		_size = st.st_size;

		if (_size < 0)
			return generateErrorPage(code);

		_body.resize(_size);
		_fd = open(_target.c_str(), O_RDONLY);
		ssize_t readSize = read(_fd, _body.data(), _size);

		if (readSize < 0)
			return generateErrorPage(code);

		defineMimeType();
	}
	else
		generateErrorPage(code);
}

void ErrorPageSource::generateErrorPage(int code) {
	std::string html = PageGenerator::generateErrorPage(code);

	_body.assign(html.begin(), html.end());
	_code = code;
	_generated = true;
	_bytesToSend = _body.size();
	_size = _body.size();
	_mime = _mimeTypes.find(".html")->second;
	_doneReading = true;
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
