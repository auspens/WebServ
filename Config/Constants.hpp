/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Constants.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wpepping <wpepping@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 14:52:12 by wouter            #+#    #+#             */
/*   Updated: 2025/06/21 16:17:34 by wpepping         ###   ########.fr       */
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

#define DEFAULT_PORT 3490
#define DEFAULT_AUTO_INDEX false
#define DEFAULT_CLIENT_MAX_BODY_SIZE 10ULL*1024*1024*1024
#define DEFAULT_ACCEPT_METHOD METHOD_GET
#define DEFAULT_CHUNK_SIZE 1024
#define DEFAULT_CONNECTION_TIMEOUT 60

#define TIMEOUT_CLEANUP_INTERVAL 30

const std::string CGI_EXTENSIONS[] = {
	".py",
	".php"
};

enum LogLevel {
	LOG_DETAIL,
	LOG_DEBUG,
	LOG_INFO,
	LOG_WARNING,
	LOG_ERROR
};

#define LOG_LEVEL LOG_DEBUG
