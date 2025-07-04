/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SourceFactory.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wpepping <wpepping@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 16:03:12 by auspensk          #+#    #+#             */
/*   Updated: 2025/07/04 15:38:08 by wpepping         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "SourceFactory.hpp"

Source *SourceFactory::getNewSource(
	const ServerConfig &serverConfig,
	const Location *location,
	HttpRequest &req
) throw(SourceAndRequestException, ChildProcessNeededException) {
	Logger::debug() << "Location: " << (location? location->getPath():serverConfig.getRootFolder()) << std::endl;
	Logger::debug() << "request path:" << req.path << std::endl;
	Logger::debug() << std::boolalpha << "POST allowed: " << (Config::getAcceptMethod(serverConfig, location) & METHOD_POST) << std::endl;
	Logger::debug() << std::boolalpha << "Upload pass: " << (location? location->isUploadPass() : false) << std::endl;

	if (location && location->isRedirect()) {
		return new RedirectSource(serverConfig, *location, req);
	}
	if (_isCgiRequest(serverConfig, location, req.path)) {
		Logger::debug() <<"CGISource is being created" <<std::endl;
		CGISource* ptr = new CGISource(serverConfig, location, req);
		return ptr;
	}
	if (_isUploadRequest(serverConfig, location, req)){
		Logger::debug() <<"UploadSource is being created" <<std::endl;
		UploadSource* ptr = new UploadSource(serverConfig, location, req);
		return ptr;
	}
	return new StaticFileSource(serverConfig, location, req);
}

Source *SourceFactory::getNewErrorPageSource(
	const ServerConfig &serverConfig,
	const Location *location,
	HttpRequest &req,
	int code
) {
	return new ErrorPageSource(serverConfig, location, req, code);
}

bool SourceFactory::_isCgiRequest(const ServerConfig &serverConfig, const Location *location, const std::string &path) {
	std::vector<std::string> acceptCgi = Config::getAcceptCgi(serverConfig, location);
	Logger::debug()<<"AcceptCgi size: " << acceptCgi.size() << " AcceptCgi at 0: " << (acceptCgi.empty() ? "null" : acceptCgi.at(0)) << std::endl;
	for (size_t i = 0; i < acceptCgi.size(); i++) {
		if (WebServUtils::strEndsWith(path, acceptCgi[i]))
			return true;
	}
	return false;
}

bool SourceFactory::_isUploadRequest(const ServerConfig &serverConfig, const Location *location, const HttpRequest &request){
	if (!location || !location->isUploadPass() || request.method != "POST")
		return false;
	std::map<std::string, std::string>::const_iterator it = request.headers.find("Content-Type");
	if (it == request.headers.end() || it->second.find("multipart/form-data") == std::string::npos)
		return false;
	int _acceptMethod = Config::getAcceptMethod(serverConfig, location);
	if (!(_acceptMethod & METHOD_POST))
		throw (SourceAndRequestException("Upload not allowed here", 403));
	if (!location)
		return false;
	if (!location->isUploadPass())
		return false;
	return true;
}
