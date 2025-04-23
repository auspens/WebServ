/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: auspensk <auspensk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 10:42:49 by auspensk          #+#    #+#             */
/*   Updated: 2025/04/23 17:11:12 by auspensk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "SystemCallsUtilities.hpp"


class Socket : public SystemCallsUtilities{

	protected:
		int _fd;
		struct addrinfo *_servinfo;

		Socket &operator=(const Socket &other);
		Socket(const Socket &other);
		Socket();

	public:
		Socket(int fd, struct addrinfo *addrinfo);
		virtual ~Socket();
		int getFd();
};
