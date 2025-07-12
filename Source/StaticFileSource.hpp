/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   StaticFileSource.hpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wouter <wouter@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 10:08:43 by auspensk          #+#    #+#             */
/*   Updated: 2025/07/12 18:08:24 by wouter           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Config.hpp"
#include "Logger.hpp"
#include "PageGenerator.hpp"
#include "Source.hpp"
#include "SystemCallsUtilities.hpp"
#include "WebServUtils.hpp"
#include "ServerConfig.hpp"

class StaticFileSource : public Source {
	public:
		void readSource() throw(SourceAndRequestException);
		char *readFromBuffer();
		void init() throw(SourceAndRequestException);

		StaticFileSource(const ServerConfig &serverConfig, Location const *location, HttpRequest &req);
		StaticFileSource(const StaticFileSource &src);
		StaticFileSource &operator=(StaticFileSource const &other);
		~StaticFileSource();

	protected:
		bool _generated;
		void checkIfDirectory() throw(SourceAndRequestException);
		void defineMimeType();
		void generateIndex() throw(SourceAndRequestException);
		void generateErrorPage(int code);
		bool indexExists(const std::vector<std::string> &indexes, const std::string &root);
};
