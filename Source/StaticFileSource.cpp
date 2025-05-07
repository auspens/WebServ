/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   StaticFileSource.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: auspensk <auspensk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 10:20:27 by auspensk          #+#    #+#             */
/*   Updated: 2025/05/07 10:16:20 by auspensk         ###   ########.fr       */
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
		}

StaticFileSource::~StaticFileSource(){
	std::cout << "FileSource destructor called";
	// close(_fd);
}

// buffer = read(10); // buffer << "1234567890"
// bytes_sent = send(fd, buffer, 10); // bytes_sent = 5
// buffer = substr(buffer, 5, 5);

// if (!dataLeftInBuffer())
// {
// 	bytesRead = read();
// 	if (!bytesRead) {
// 		close();
// 		return;
// 	}
// }
// bytes = socket.send();
// bytesSent(bytes);


// void StaticFileSource::bytesSent(int bytes) { }

void StaticFileSource::readSource(){
	if (_bytesToSend > 0)
		return;
	_body.clear();
	_body.resize(_serverConfig.getBufferSize());
	ssize_t readSize = read(_fd, _body.data(),_serverConfig.getBufferSize());
	if (readSize < 0)
		throw Source::SourceException("Could not read from static source file");
	_bytesToSend = readSize;
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
	if(access(_target.c_str(), R_OK))
		_target = getErrorPage(404);
	if(access(_target.c_str(), R_OK))
		throw (Source::SourceException("No 404 page"));
}

std::string StaticFileSource::generateIndex()const{
	//placeholder!!
	std::string index("generated index");
	return index;
}

std::string StaticFileSource::getErrorPage(int code)const{
	//doesn't handle cases when error directive uses external URLs, like: error_page 404 https://example.com/notfound.html
	//I'm not sure if we need to include this feature. Doesn't say anything in the subject
	if (!_location.getErorrPagesLocal().empty() && _location.getErorrPagesLocal().find(code) != _location.getErorrPagesLocal().end())
		return (_serverConfig.getRootFolder() + _location.getPath() + _location.getErorrPagesLocal().find(code)->second);
	if (_serverConfig.getErrorPages().find(code) != _serverConfig.getErrorPages().end())
		return (_serverConfig.getRootFolder() + _serverConfig.getErrorPages().find(code)->second);
	throw (Source::SourceException("No erorr page"));
}
