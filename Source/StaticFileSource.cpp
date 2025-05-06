/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   StaticFileSource.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: auspensk <auspensk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 10:20:27 by auspensk          #+#    #+#             */
/*   Updated: 2025/05/05 16:37:43 by auspensk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "StaticFileSource.hpp"

StaticFileSource::StaticFileSource(const std::string &target, const ServerConfig &serverConfig)
		: Source(target, serverConfig){}

StaticFileSource::~StaticFileSource(){}

void StaticFileSource::read(){
	checkIfDirectory();
	checkIfExists();
	std::ifstream file(_target.c_str(), std::ios::binary | std::ios::ate);
	if (!file)
		throw Source::SourceException("Could not open filestream");
	_size = file.tellg();
	file.seekg(0, std::ios::beg);
	_bytesRead.resize(_size);
	if(!file.read(_bytesRead.data(), _size).good())
		throw Source::SourceException("Could not read from filestream");
}

bool StaticFileSource::checkForRedirections(){
	if (_location.getRedirect().first != 0){
		_code = _location.getRedirect().first;
		_target = _location.getRedirect().second;
		return true;
	}
	return false;
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
