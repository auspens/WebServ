/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   StaticFileSource.hpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: auspensk <auspensk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 10:08:43 by auspensk          #+#    #+#             */
/*   Updated: 2025/05/16 17:32:29 by auspensk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "Source.hpp"
#include "SystemCallsUtilities.hpp"
#include "WebServUtils.hpp"
#include "ServerConfig.hpp"
#include <dirent.h>

class StaticFileSource : public Source {
	public:
		void readSource();
		char *readFromSource();//returns a buffer that can be sent through socket

		StaticFileSource(const std::string &target, const ServerConfig &serverConfig, Location const &location);
		~StaticFileSource();

	private:
		void checkIfExists();
		void checkIfDirectory();
		void defineMimeType();
		std::string generateIndex()const;
		std::string getErrorPage(int index)const;
};
