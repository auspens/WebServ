/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ErrorPageSource.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wpepping <wpepping@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/06 15:37:02 by auspensk          #+#    #+#             */
/*   Updated: 2025/07/17 14:57:02 by wpepping         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Logger.hpp"
#include "StaticFileSource.hpp"
#include "WebServUtils.hpp"


class ErrorPageSource : public StaticFileSource{
	public:
		ErrorPageSource(const ServerConfig &serverConfig, Location const *location, HttpRequest &req, int code);
		ErrorPageSource(const ErrorPageSource &src);
		ErrorPageSource &operator=(const ErrorPageSource &other);
		~ErrorPageSource();
		void readSource() throw(SourceAndRequestException);

		void getErrorPage(int index);
		void generateErrorPage(int code);
		void init() throw(SourceAndRequestException);
	private:
		int _code;
		void setHeader();
};

