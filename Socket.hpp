/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wouter <wouter@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 10:42:49 by auspensk          #+#    #+#             */
/*   Updated: 2025/04/26 17:54:12 by wouter           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "SystemCallsUtilities.hpp"


class Socket {
	protected:
		int _fd;
		struct addrinfo *_addrinfo;

	public:
		Socket();
		Socket(int fd);
		Socket(int fd, struct addrinfo *addrinfo);
		Socket(const Socket &other);
		~Socket();
		Socket &operator=(const Socket &other);

		int		getFd() const;
		void	close_sock();
};
