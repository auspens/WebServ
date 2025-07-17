/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ShutDownSource.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: auspensk <auspensk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/06 17:45:35 by wouter            #+#    #+#             */
/*   Updated: 2025/07/16 14:57:57 by auspensk         ###   ########.fr       */
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
		ShutDownSource(const ShutDownSource &other);
		ShutDownSource &operator=(const ShutDownSource &other);

		void readSource();
		void init() throw(SourceAndRequestException);
		void setHeader();
};



