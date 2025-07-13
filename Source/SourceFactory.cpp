/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SourceFactory.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wouter <wouter@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 16:03:12 by auspensk          #+#    #+#             */
/*   Updated: 2025/07/13 16:58:31 by wouter           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "SourceFactory.hpp"

Source *SourceFactory::getNewSource(
	const ServerConfig &serverConfig,
	const Location *location,
	HttpRequest &req,
	bool &shutDownFlag
) throw(SourceAndRequestException, IsChildProcessException) {
	Logger::debug() << "Location: " << (location? location->getPath():serverConfig.getRootFolder()) << std::endl;
	Logger::debug() << "request path:" << req.path << std::endl;
	Logger::debug() << "method: " << req.method << std::endl;
	Logger::debug() << std::boolalpha << "method allowed: " << Config::acceptsMethod(serverConfig, location, req.method) << std::endl;
	Logger::debug() << std::boolalpha << "Upload pass: " << (location? location->isUploadPass() : false) << std::endl;

	std::string target = _findTarget(serverConfig, location, req);

	if (location && location->isRedirect() && target == location->getPath())
		return new RedirectSource(serverConfig, *location, req);

	// Moved this back. Won't cause loop because errors go through getNewErrorPageSource.
	if (!target.empty() && access(target.c_str(), F_OK) != 0)
		throw(SourceAndRequestException("Not found", 404));
	if (!Config::acceptsMethod(serverConfig, location, req.method))
		throw(SourceAndRequestException("Method not allowed", 405));

	if (_isCgiRequest(serverConfig, location, target))
		return new CGISource(serverConfig, location, req, target);
	if (_isUploadRequest(serverConfig, location, req))
		return new UploadSource(serverConfig, location, req, target);
	if (_isDeleteRequest(req))
		return new DeleteSource(serverConfig, *location, req, target);
	if (_isShutDownRequest(location, req)) {
		shutDownFlag = true;
		return new ShutDownSource(serverConfig, *location, req);
	}
	if (req.method == "GET")
		return new StaticFileSource(serverConfig, location, req, target);
	throw SourceAndRequestException("Method not allowed", 405);
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
	return !WebServUtils::getCgiExtension(path, Config::getAcceptCgi(serverConfig, location)).empty();
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

bool SourceFactory::_isDeleteRequest(const HttpRequest &request) {
	return request.method == "DELETE";
}

bool SourceFactory::_isShutDownRequest(const Location *location, const HttpRequest &request) {
	return request.method == "GET" && location && location->isShutDown();
}

std::string SourceFactory::_findTarget(const ServerConfig &serverConfig, const Location *location, HttpRequest req) {
	std::string						target;
	std::string						index_target;
	const std::vector<std::string>	indexes = Config::getIndexPages(serverConfig, location);

	if (location)
		target = WebServUtils::pathJoin(location->getRootFolder(), req.path.substr(location->getPath().size()));
	else
		target = WebServUtils::pathJoin(serverConfig.getRootFolder(), req.path);

	if (WebServUtils::folderExists(target) && req.method == "GET") {
		for (size_t i = 0; i < indexes.size(); ++i){
			index_target = WebServUtils::pathJoin(target, indexes.at(i));
			if (access(index_target.c_str(), F_OK) == 0)
				return index_target;
		}
	}
	return target;
}
