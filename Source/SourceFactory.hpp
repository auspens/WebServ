/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SourceFactory.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wpepping <wpepping@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 16:00:07 by auspensk          #+#    #+#             */
/*   Updated: 2025/07/04 15:37:34 by wpepping         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "CGISource.hpp"
#include "ChildProcessNeededException.hpp"
#include "Constants.hpp"
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
};
