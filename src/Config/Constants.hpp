/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Constants.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wpepping <wpepping@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 14:52:12 by wouter            #+#    #+#             */
/*   Updated: 2025/07/17 15:31:32 by wpepping         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <string>

enum eHttpMethod {
		METHOD_NONE   = 0,
		METHOD_GET    = 1 << 0, // 0001
		METHOD_POST   = 1 << 1, // 0010
		METHOD_DELETE = 1 << 2  // 0100
};

enum LogLevel {
	LOG_DETAIL,
	LOG_DEBUG,
	LOG_INFO,
	LOG_WARNING,
	LOG_ERROR
};

#define PROTOCOL "HTTP/1.1"

#define DEFAULT_PORT 3490
#define DEFAULT_AUTO_INDEX false
#define DEFAULT_client_max_request_size 10ULL*1024*1024*1024
#define DEFAULT_ACCEPT_METHOD METHOD_GET
#define DEFAULT_CHUNK_SIZE 1024
#define DEFAULT_CONNECTION_TIMEOUT 60
#define DEFAULT_CGI_TIMEOUT 60
#define DEFAULT_ALLOW_DELETE false
#define DEFAULT_INDEX "index.html"
#define DEFAULT_PYTHON_EXECUTABLE "/usr/bin/python3"

#define TIMEOUT_CLEANUP_INTERVAL 1
#define DOCSTRING "<!DOCTYPE html>\n"
#define DEFAULT_PERMISSIONS 0644

#define STATUS_CODES_JSON "metadata/StatusCodes.json"
#define MIME_TYPES_JSON "metadata/FileExtensionsToMime.json"

const std::string CGI_EXTENSIONS[] = {
	".py",
	".php"
};

#define LOG_LEVEL LOG_INFO
