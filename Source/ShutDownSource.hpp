/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ShutDownSource.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: auspensk <auspensk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/06 17:45:35 by wouter            #+#    #+#             */
/*   Updated: 2025/07/11 13:52:50 by auspensk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <cstdio>
#include "Logger.hpp"
#include "Source.hpp"

class ShutDownSource : public Source
{
	public:
		ShutDownSource(
			const ServerConfig &serverConfig,
			const Location &location,
			HttpRequest &req
		);
		~ShutDownSource();

		void readSource();
		void setHeader(std::string header);
		void init(const ServerConfig &serverConfig, const Location *location, HttpRequest &req) throw(SourceAndRequestException);
};



