/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SourceFactory.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: auspensk <auspensk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 16:00:07 by auspensk          #+#    #+#             */
/*   Updated: 2025/07/08 16:00:01 by auspensk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "CGISource.hpp"
#include "ChildProcessNeededException.hpp"
#include "../Config/Constants.hpp"
#include "ErrorPageSource.hpp"
#include "RedirectSource.hpp"
#include "SourceAndRequestException.hpp"
#include "StaticFileSource.hpp"
#include "UploadSource.hpp"
#include "DeleteSource.hpp"

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
		static bool _isUploadRequest(const Location *location, const HttpRequest &request);
		static bool _isDeleteRequest(const Location *location, const HttpRequest &request);
};
