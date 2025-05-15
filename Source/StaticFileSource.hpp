/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   StaticFileSource.hpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wpepping <wpepping@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 10:08:43 by auspensk          #+#    #+#             */
/*   Updated: 2025/05/15 15:10:55 by wpepping         ###   ########.fr       */
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
		StaticFileSource(const std::string &target, const ServerConfig &serverConfig, Location const &location);
		~StaticFileSource();

	private:
		void checkIfExists();
		void checkIfDirectory();
		std::string generateIndex()const;
		std::string getErrorPage(int index)const;
};
