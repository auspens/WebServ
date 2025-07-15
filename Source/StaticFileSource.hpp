/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   StaticFileSource.hpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: auspensk <auspensk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 10:08:43 by auspensk          #+#    #+#             */
/*   Updated: 2025/07/15 11:16:18 by auspensk         ###   ########.fr       */
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

		StaticFileSource(
			const ServerConfig &serverConfig,
			Location const *location,
			HttpRequest &req,
			std::string target
		);
		StaticFileSource(const StaticFileSource &src);
		StaticFileSource &operator=(StaticFileSource const &other);
		~StaticFileSource();

	protected:
		bool _generated;
		void checkAutoIndex() throw(SourceAndRequestException);
		void defineMimeType();
		void generateIndex() throw(SourceAndRequestException);
};
