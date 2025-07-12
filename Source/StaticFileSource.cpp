/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   StaticFileSource.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wouter <wouter@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 10:20:27 by auspensk          #+#    #+#             */
/*   Updated: 2025/07/12 19:13:00 by wouter           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "StaticFileSource.hpp"

StaticFileSource::StaticFileSource(const ServerConfig &serverConfig, Location const *location, HttpRequest &req)
	: Source(serverConfig, location, req)
	, _generated(false) {
	Logger::debug() << "Creating Static File Source for " << _target << std::endl;
	_location = location;
}

void StaticFileSource::init() throw(SourceAndRequestException) {
	Source::init();
	_type = STATIC;
	checkIfDirectory();
	if (!_generated) {
		_checkAccess(_target);
		_fd = open(_target.c_str(), O_RDONLY);
		struct stat st;
		stat(_target.c_str(), &st);
		_size = st.st_size;
		defineMimeType();
	}
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
		std::string target = WebServUtils::pathJoin(root, WebServUtils::pathJoin(_request.path, indexes.at(i)));
		if (_checkExists(target)) {
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

	if (indexExists(Config::getIndexPages(_serverConfig, _location), Config::getRootFolder(_serverConfig, _location)))
		return;
	if (Config::getAutoIndex(_serverConfig, _location)){
		generateIndex();
		return;
	}

	throw SourceAndRequestException("No index page for this directory and autoindex is off", 404);
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

void StaticFileSource::generateIndex() throw(SourceAndRequestException) {
	std::string	relativeTarget = _request.path;
	std::string	folder;
	bool		acceptsDelete = Config::acceptsMethod(_serverConfig, _location, METHOD_DELETE);

	if (_location)
		relativeTarget.erase(0, _location->getPath().length());
	folder = WebServUtils::pathJoin(Config::getRootFolder(_serverConfig, _location), relativeTarget);

	std::string html = PageGenerator::generateIndex(_request.path, folder, acceptsDelete);

	_body.assign(html.begin(), html.end());
	_generated = true;
	_bytesToSend = _body.size();
	_size = _body.size();
	_mime = _mimeTypes.find(".html")->second;
	_doneReading = true;
}

void StaticFileSource::generateErrorPage(int code){
	_code = code;
	_generated = true;
	std::string html = PageGenerator::generateErrorPage(code);
	_body.assign(html.begin(), html.end());
	_generated = true;
	_bytesToSend = _body.size();
	_size = _body.size();
	_mime = _mimeTypes.find(".html")->second;
	_doneReading = true;
}

char *StaticFileSource::readFromBuffer(){
	return Source::readFromBuffer();
}
