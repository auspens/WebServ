/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SourceFactory.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: auspensk <auspensk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 16:00:07 by auspensk          #+#    #+#             */
/*   Updated: 2025/06/11 16:05:47 by auspensk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "StaticFileSource.hpp"
#include "UploadSource.hpp"
#include "RedirectSource.hpp"
#include "ErrorPageSource.hpp"
#include "CGISource.hpp"
#include "SourceAndRequestException.hpp"
#include "ChildProcessNeededException.hpp"

class SourceFactory {
	public:
		static Source *getNewSource(
			const ServerConfig &serverConfig,
			HttpRequest req
		) throw(SourceAndRequestException, ChildProcessNeededException);
		static Source *getNewErrorPageSource(
			const ServerConfig &serverConfig,
			HttpRequest req,
			int code
		);
	private:
		static const Location *_findLocation (
			const std::string &target,
			const ServerConfig &serverConfig
		);
		static bool _isCgiRequest(const ServerConfig &serverConfig, const Location *location, const std::string &path);
};
