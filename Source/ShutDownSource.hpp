/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ShutDownSource.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wpepping <wpepping@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/06 17:45:35 by wouter            #+#    #+#             */
/*   Updated: 2025/07/11 17:09:08 by wpepping         ###   ########.fr       */
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
		void init() throw(SourceAndRequestException);
};



