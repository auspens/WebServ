/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   DeleteSource.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wouter <wouter@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/06 17:45:35 by wouter            #+#    #+#             */
/*   Updated: 2025/07/06 18:54:40 by wouter           ###   ########.fr       */
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

		void readSource();

	private:
		void _deleteFile() throw(SourceAndRequestException);
		void _setResponse();
};



