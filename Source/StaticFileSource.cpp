/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   StaticFileSource.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wpepping <wpepping@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 10:20:27 by auspensk          #+#    #+#             */
/*   Updated: 2025/07/16 16:47:14 by wpepping         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "StaticFileSource.hpp"

StaticFileSource::StaticFileSource(
	const ServerConfig &serverConfig,
	Location const *location,
	HttpRequest &req,
	std::string target
)
	: Source(serverConfig, location, req)
	, _generated(false) {
	Logger::debug() << "Creating Static File Source for " << target << std::endl;
	_location = location;
	_target = target;
}

void StaticFileSource::init() throw(SourceAndRequestException) {
	Source::init();

	if (WebServUtils::folderExists(_target))
		checkAutoIndex();

	if (!_generated) {
		_checkAccess(_target);
		_fd = open(_target.c_str(), O_RDONLY);
		struct stat st;
		stat(_target.c_str(), &st);
		_size = st.st_size;
		defineMimeType();
	}
	setHeader();
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

void StaticFileSource::checkAutoIndex() throw(SourceAndRequestException) {
	if (Config::getAutoIndex(_serverConfig, _location))
		generateIndex();
	else
		throw SourceAndRequestException("No index page for this directory and autoindex is off", 404);
}

void StaticFileSource::defineMimeType() {
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
	bool		acceptsDelete = Config::acceptsMethod(_serverConfig, _location, METHOD_DELETE);
	std::string	html = PageGenerator::generateIndex(_request.path, _target, acceptsDelete);

	_body.assign(html.begin(), html.end());
	_bytesToSend = html.size();
	_size = _body.size();
	_mime = _mimeTypes.find(".html")->second;
	_generated = true;
	_doneReading = true;
}

char *StaticFileSource::readFromBuffer(){
	return Source::readFromBuffer();
}
