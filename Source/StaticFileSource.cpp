/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   StaticFileSource.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wpepping <wpepping@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 10:20:27 by auspensk          #+#    #+#             */
/*   Updated: 2025/05/23 17:53:35 by wpepping         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "StaticFileSource.hpp"

StaticFileSource::StaticFileSource(const std::string &target, const ServerConfig &serverConfig, Location const &location)
		: Source(target, serverConfig){
			_location = location;
			checkIfDirectory();
			checkIfExists();
			_fd = open(_target.c_str(), O_RDONLY);
			if (_fd < 0)
				throw Source::SourceException("Could not open static source file");
			struct stat st;
			stat(_target.c_str(), &st);
			_size = st.st_size;
			defineMimeType();
		}

StaticFileSource::~StaticFileSource(){
	close(_fd);
}

void StaticFileSource::readSource(){
	if (_bytesToSend > 0)
		return;
	_body.clear();
	_body.resize(_serverConfig.getBufferSize());
	_offset = 0;
	ssize_t readSize = read(_fd, _body.data(),_serverConfig.getBufferSize());
	if (readSize < 0)
		throw Source::SourceException("Could not read from static source file");
	_bytesToSend = readSize;
	_body.resize(readSize);
}

void StaticFileSource::checkIfDirectory(){
	if (_target.at(_target.size()-1) != '/')
		return ;
	if (!_location.getIndex().empty()){
		_target += _location.getIndex();
		return;
	}
	if (_location.autoindexOn()){
		_target += generateIndex();
		return ;
	}
	_code = 403;
	_target = getErrorPage(403);
}

void StaticFileSource::checkIfExists(){
	DIR *dir = opendir(_serverConfig.getRootFolder().c_str());
	if (!dir)
		throw (Source::SourceException("No root folder"));
	closedir(dir);
	if(access(_target.c_str(), R_OK)){
		_code = 404;
		_target = getErrorPage(404);
	}
	if(access(_target.c_str(), R_OK))
		throw (Source::SourceException("No 404 page"));
}

void StaticFileSource::defineMimeType(){
	int dotAt = _target.find_last_of('.');
	std::string extension = _target.substr(dotAt);
	_mime = _mimeTypes.find(extension)->second;
}


std::string StaticFileSource::generateIndex()const{
	//placeholder!!
	std::string index("generated index");
	return index;
}

std::string StaticFileSource::getErrorPage(int code)const{
	//doesn't handle cases when error directive uses external URLs, like: error_page 404 https://example.com/notfound.html
	//I'm not sure if we need to include this feature. Doesn't say anything in the subject
	if (!_location.getErrorPages().empty() && _location.getErrorPages().find(code) != _location.getErrorPages().end())
		return (_serverConfig.getRootFolder() + _location.getPath() + _location.getErrorPages().find(code)->second);
	if (_serverConfig.getErrorPages().find(code) != _serverConfig.getErrorPages().end())
		return (_serverConfig.getRootFolder() + _serverConfig.getErrorPages().find(code)->second);
	std::string current_error = "/error/404.html";
	return (_serverConfig.getRootFolder() + current_error);
	throw (Source::SourceException("No error page"));
}

char *StaticFileSource::readFromSource(){
	readSource();
	return readFromBuffer();
}
