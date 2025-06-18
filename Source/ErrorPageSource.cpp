/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ErrorPageSource.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: auspensk <auspensk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/06 15:55:07 by auspensk          #+#    #+#             */
/*   Updated: 2025/06/18 16:09:38 by auspensk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ErrorPageSource.hpp"

// Source *source;

// try {
// 	source = new StaticFileSource(serverConfig, location, req, path);
// } catch (SomethingException &e) {
// 	source = new GeneratedSource(serverConfig, location, req, body);
// 	source->generatePage();
// }

// UploadSource() : source(NULL) {

// }


// source = new GeneratedSource(serverConfig, location, req, body);
// source->generatePage();

// void readFromSource() {
// 	if (source)
// 		source->readFromSource();
// }

ErrorPageSource::ErrorPageSource
		(const ServerConfig &serverConfig,
		Location const *location,
		HttpRequest req, int code)
		:	StaticFileSource(serverConfig, location, req, code){ //this constructor is specific for ErrorPage and skips all usual checks for static file to avoid looping in error
	_code = code;
	_body.clear();
	getErrorPage(code);
	_body.resize(_size);
	_offset = 0;
	_bytesToSend = _size;
	_doneReading = true;
	Logger::debug() << "Body: \n" << std::endl;
	WebServUtils::printVector(_body);
}

void ErrorPageSource::getErrorPage(int code){
	// doesn't handle cases when error directive uses external URLs, like: error_page 404 https://example.com/notfound.html
	// I'm not sure if we need to include this feature. Doesn't say anything in the subject
	if (Config::getErrorPages(_serverConfig, _location).find(code) != Config::getErrorPages(_serverConfig, _location).end()){
		_target = Config::getErrorPages(_serverConfig, _location).find(code)->second;
		_fd = open(_target.c_str(), O_RDONLY);
		struct stat st;
		stat(_target.c_str(), &st);
		_size = st.st_size;
		_body.resize(_size);
		ssize_t readSize = read(_fd, _body.data(), _size);
		Logger::debug() << "Body after read: \n" << std::endl;
		WebServUtils::printVector(_body);
		if (readSize < 0)
			return generatePage(code);
		defineMimeType();
	}
	else
		generatePage(code);
	Logger::debug() << "Body in generate page: \n" << std::endl;
	WebServUtils::printVector(_body);
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
