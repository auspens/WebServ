/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SourceFactory.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: auspensk <auspensk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 16:03:12 by auspensk          #+#    #+#             */
/*   Updated: 2025/06/17 15:41:22 by auspensk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "SourceFactory.hpp"

Source *SourceFactory::getNewSource(const ServerConfig &serverConfig, HttpRequest req) throw(SourceAndRequestException, ChildProcessNeededException) {
	const Location *location = _findLocation(req.path, serverConfig);
	std::cout << "Location: " << (location? location->getPath():serverConfig.getRootFolder()) << std::endl;
	std::cout << "request path:" << req.path << std::endl;
	std::cout << std::boolalpha << "POST allowed: " << (Config::getAcceptMethod(serverConfig, location) & METHOD_POST) << std::endl;
	std::cout << std::boolalpha << "Upload pass: " << (location? location->isUploadPass() : false) << std::endl;

	if (location && location->isRedirect()) {
		return new RedirectSource(serverConfig, *location, req);
	}
	if (_isCgiRequest(serverConfig, location, req.path)) {
		CGISource* ptr = new CGISource(serverConfig, location, req);
			return ptr;
	}
	if (_isUploadRequest(serverConfig, location, req)){
		UploadSource* ptr = new UploadSource(serverConfig, location, req);
		return ptr;
	}
	return new StaticFileSource(serverConfig, location, req);
}

Source *SourceFactory::getNewErrorPageSource(const ServerConfig &serverConfig, HttpRequest req, int code) {
	const Location *location = _findLocation(req.path, serverConfig);
	return new ErrorPageSource(serverConfig, location, req, code);
}

const Location *SourceFactory::_findLocation (
	const std::string &target,
	const ServerConfig &serverConfig
) {
	const std::vector<Location *> locations = serverConfig.getLocations();
	std::vector<Location *>::const_iterator it;

	for (it = locations.begin(); it != locations.end(); ++it) {
		std::string locationPath = (*it)->getPath();

		if (target.compare(0, locationPath.size(), locationPath) == 0
			&& (target.size() == locationPath.size()
			|| WebServUtils::isin("/#?", target.at(locationPath.size()))))
			return *it;
	}
	return (NULL);
}

bool SourceFactory::_isCgiRequest(const ServerConfig &serverConfig, const Location *location, const std::string &path) {
	std::vector<std::string> acceptCgi = Config::getAcceptCgi(serverConfig, location);
	for (size_t i = 0; i < acceptCgi.size(); i++) {
		if (WebServUtils::strEndsWith(path, acceptCgi[i]))
			return true;
	}
	return false;
}

bool SourceFactory::_isUploadRequest(const ServerConfig &serverConfig, const Location *location, const HttpRequest &request){
	if (request.method != "POST")
		return false;
	std::map<std::string, std::string>::const_iterator it = request.headers.find("Content-Type: multipart/form-data");
	if (it == request.headers.end())
		return false;
	int _acceptMethod = Config::getAcceptMethod(serverConfig, location);
	if (_acceptMethod & METHOD_POST)
		return true;
	throw (SourceAndRequestException("Upload not allowed here", 403));
}
