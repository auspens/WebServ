/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   StaticFileSource.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wpepping <wpepping@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 10:20:27 by auspensk          #+#    #+#             */
/*   Updated: 2025/07/04 17:26:58 by wpepping         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "StaticFileSource.hpp"

StaticFileSource::StaticFileSource(const ServerConfig &serverConfig, Location const *location, HttpRequest &req)
	: Source(serverConfig, location, req)
	, _generated(false){
	Logger::debug() << "Creating Static File Source for " << _target << std::endl;
	_location = location;
	checkIfDirectory();
	if (!_generated && !checkIfExists(_target))
		throw (SourceAndRequestException("Page doesn't exist", 404));
	if (!_generated){
		_fd = open(_target.c_str(), O_RDONLY);
		struct stat st;
		stat(_target.c_str(), &st);
		_size = st.st_size;
		defineMimeType();
	}
}
StaticFileSource::StaticFileSource(const ServerConfig &serverConfig, Location const *location, HttpRequest &req, int code)throw()
	: Source(serverConfig, location, req, code)
	, _generated(false){
	_location = location;
}

StaticFileSource::StaticFileSource(const StaticFileSource &src):
	Source(src),
	_generated(src._generated){}
StaticFileSource &StaticFileSource::operator=(StaticFileSource const &other){
	if (this != &other){
		Source::operator=(other);
		_generated = other._generated;
	}
	return *this;
}
StaticFileSource::~StaticFileSource(){
	close(_fd);
}

void StaticFileSource::readSource() throw(SourceAndRequestException) {
	if (_bytesToSend == 0 && !_generated && !_doneReading) {
		ssize_t readSize = read(_fd, _body.data(), _serverConfig.getBufferSize());
		if (readSize < 0)
			throw SourceAndRequestException("Could not read from static source file", 500);
		if (readSize == 0)
			_doneReading = true;
		_bytesToSend = readSize;
		_offset = 0;
	}
}

bool StaticFileSource::indexExists(const std::vector<std::string> &indexes, const std::string &root){
	for (size_t i = 0; i < indexes.size(); ++i){
		std::string target = root + std::string("/") + indexes.at(i);
		if (checkIfExists(target)){
				_target = target;
				return true;
		}
	}
	return false;
}

void StaticFileSource::checkIfDirectory() throw(SourceAndRequestException) {
	DIR *dir = opendir(_target.c_str());
	if (!dir)
		return;
	closedir(dir);
	if (!_location){
		if (indexExists(_serverConfig.getIndexPages(), _serverConfig.getRootFolder()))
			return;
		if (_serverConfig.getAutoIndex()){
			generateIndex();
			return;
		}
	}
	else{
		if(indexExists(_location->getIndexPages(),_location->getPath()))
				return;
		if (_location->getAutoIndex()){
			generateIndex();
			return;
		}
	}
	throw SourceAndRequestException("No index page for this directory and autoindex is off", 404);
}

bool StaticFileSource::checkIfExists(std::string &target){
	DIR *dir = opendir(_serverConfig.getRootFolder().c_str());
	if (!dir)
		throw(SourceAndRequestException("No root folder", 404));
	closedir(dir);
	if (access(target.c_str(), R_OK))
		return false;
	return true;
}

void StaticFileSource::defineMimeType(){
	int dotAt = _target.find_last_of('.');
	if (dotAt < 0){
		_mime = "application/octet-stream";
		return ;
	}
	std::string extension = _target.substr(dotAt);
	std::map<std::string, std::string>::iterator it = _mimeTypes.find(extension);
	if (it != _mimeTypes.end())
		_mime = it->second;
	else
		_mime = "application/octet-stream";
}

bool StaticFileSource::readDirectories(std::vector<DirEntry> &entries){
	Logger::debug() << "Target: " << _target << std::endl;
	DIR *dir = opendir(_target.c_str());
	if (!dir)
		return false;
	struct dirent *entry;
	while ((entry = readdir(dir)) != NULL){
		std::string name = entry->d_name;
		if (name == "." || name == "..")
			continue;

		std::string full_path = _target + "/" + name;
		struct stat s;
		if (stat(full_path.c_str(), &s) == 0){
			bool is_dir = S_ISDIR(s.st_mode);
			entries.push_back(DirEntry(name, is_dir));
		}
	}
	closedir(dir);
	return true;
}

void StaticFileSource::generateIndex() throw(SourceAndRequestException) {
	std::vector<DirEntry> entries;
	if (!readDirectories(entries))
		throw SourceAndRequestException("Could not generate index", 500);
	std::string relativeTarget = _target;
	std::string prefix = _serverConfig.getRootFolder();
	if (relativeTarget.rfind(prefix, 0) == 0)
		relativeTarget.erase(0, prefix.length());

	std::string html = "<html><head><title>Index of " + relativeTarget + "</title></head>";
	html += "<body><h1> Index of " + relativeTarget + "</h1><hr><ul>";
	for (size_t i = 0; i < entries.size(); ++i){
		std::string name = entries[i].name;
		std::string href = relativeTarget + "/" + name;
		if (entries[i].is_directory){
			href += "/";
			name += "/";
		}
		html += "<li><a href=\"" + href + "\">" + name + "</a></li>";
	}
	html += "</ul><hr></body></html>";
	_body.assign(html.begin(), html.end());
	_generated = true;
	_bytesToSend = _body.size();
	_size = _body.size();
	_mime = _mimeTypes.find(".html")->second;
	_doneReading = true;
}


void StaticFileSource::generatePage(int code){
	_code = code;
	_generated = true;
	std::string html = "<html><head><title> " + _statusCodes.at(code).message + "</title></head>";
	html += "<body><h1>" + _statusCodes.at(code).code + " " + _statusCodes.at(code).message + "</h1>";
	html += "<div>" + _statusCodes.at(code).description + "</div>";
	html += "</body></html>";
	_body.assign(html.begin(), html.end());
	_generated = true;
	_bytesToSend = _body.size();
	_size = _body.size();
	_mime = _mimeTypes.find(".html")->second;
	_doneReading = true;
}

char *StaticFileSource::readFromBuffer(){
	// readSource();
	return Source::readFromBuffer();
}
