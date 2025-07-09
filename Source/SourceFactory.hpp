/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SourceFactory.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wouter <wouter@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 16:00:07 by auspensk          #+#    #+#             */
/*   Updated: 2025/07/09 20:56:34 by wouter           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "CGISource.hpp"
#include "IsChildProcessException.hpp"
#include "Constants.hpp"
#include "DeleteSource.hpp"
#include "ErrorPageSource.hpp"
#include "RedirectSource.hpp"
#include "ShutDownSource.hpp"
#include "SourceAndRequestException.hpp"
#include "StaticFileSource.hpp"
#include "UploadSource.hpp"

class SourceFactory {
	public:
		static Source *getNewSource(
			const ServerConfig &serverConfig,
			const Location *location,
			HttpRequest &req,
			bool &shutDownFlag
		) throw(SourceAndRequestException, IsChildProcessException);
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
		static bool _isShutDownRequest(const Location *location, const HttpRequest &request);
};
