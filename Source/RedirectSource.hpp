/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RedirectSource.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: auspensk <auspensk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 16:37:31 by auspensk          #+#    #+#             */
/*   Updated: 2025/05/06 18:08:42 by auspensk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "Source.hpp"

class RedirectSource : public Source
{
	private:
	public:
		RedirectSource(const std::string &target, const ServerConfig &serverConfig, int code);
		~RedirectSource();
		void readSource();
};



