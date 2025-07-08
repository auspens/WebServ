/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SourceFactory.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: auspensk <auspensk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 16:03:12 by auspensk          #+#    #+#             */
/*   Updated: 2025/07/08 16:11:38 by auspensk         ###   ########.fr       */
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

	if (location && location->isRedirect())
		return new RedirectSource(serverConfig, *location, req);
	else if (_isCgiRequest(serverConfig, location, req.path))
		return new CGISource(serverConfig, location, req);
	else if (_isUploadRequest(location, req))
		return new UploadSource(serverConfig, location, req);
	else if (_isDeleteRequest(location, req))
		return new DeleteSource(serverConfig, location, req);
	else
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

bool SourceFactory::_isUploadRequest(const Location *location, const HttpRequest &request){
	if (!location || !location->isUploadPass() || request.method != "POST")
		return false;
	std::map<std::string, std::string>::const_iterator it = request.headers.find("Content-Type");
	if (it == request.headers.end() || it->second.find("multipart/form-data") == std::string::npos)
		return false;
	return true;
}

bool SourceFactory::_isDeleteRequest(const Location *location, const HttpRequest &request){
	if (!location || request.method != "DELETE")
		return false;
	Logger::debug()<<"Delete request identified" << std::endl;
	return true;
}
