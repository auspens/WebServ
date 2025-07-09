/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   DeleteSource.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wouter <wouter@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/06 17:45:35 by wouter            #+#    #+#             */
/*   Updated: 2025/07/09 21:04:14 by wouter           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <cstdio>
#include "Logger.hpp"
#include "Source.hpp"

class DeleteSource : public Source
{
	public:
		DeleteSource(
			const ServerConfig &serverConfig,
			const Location &location,
			HttpRequest &req
		) throw(SourceAndRequestException);
		~DeleteSource();

		void init() throw(SourceAndRequestException);
		void readSource();
		void setHeader(std::string header);

	private:
		void _deleteFile() throw(SourceAndRequestException);
};



