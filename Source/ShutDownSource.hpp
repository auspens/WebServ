/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ShutDownSource.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wouter <wouter@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/06 17:45:35 by wouter            #+#    #+#             */
/*   Updated: 2025/07/12 21:36:50 by wouter           ###   ########.fr       */
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
		void init() throw(SourceAndRequestException);
		void setHeader();
};



