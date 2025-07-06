/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SourceFactory.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wouter <wouter@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 16:00:07 by auspensk          #+#    #+#             */
/*   Updated: 2025/07/06 18:34:47 by wouter           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "CGISource.hpp"
#include "ChildProcessNeededException.hpp"
#include "Constants.hpp"
#include "DeleteSource.hpp"
#include "ErrorPageSource.hpp"
#include "RedirectSource.hpp"
#include "SourceAndRequestException.hpp"
#include "StaticFileSource.hpp"
#include "UploadSource.hpp"

class SourceFactory {
	public:
		static Source *getNewSource(
			const ServerConfig &serverConfig,
			const Location *location,
			HttpRequest &req
		) throw(SourceAndRequestException, ChildProcessNeededException);
		static Source *getNewErrorPageSource(
			const ServerConfig &serverConfig,
			const Location *location,
			HttpRequest &req,
			int code
		);
	private:
		static bool _isCgiRequest(const ServerConfig &serverConfig, const Location *location, const std::string &path);
		static bool _isUploadRequest(const ServerConfig &serverConfig, const Location *location, const HttpRequest &request);
		static bool _isDeleteRequest(const HttpRequest &request);
};
