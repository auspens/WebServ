/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RedirectSource.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eusatiko <eusatiko@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 16:37:31 by auspensk          #+#    #+#             */
/*   Updated: 2025/05/28 14:24:51 by eusatiko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "Source.hpp"

class RedirectSource : public Source
{
	private:
	public:
		RedirectSource(const std::string &target, const ServerConfig &serverConfig, int code, HttpRequest req);
		~RedirectSource();
		void readSource();
		char *getBufferToSend();
};



