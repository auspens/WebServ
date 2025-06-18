/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wpepping <wpepping@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 10:42:49 by auspensk          #+#    #+#             */
/*   Updated: 2025/06/17 18:59:31 by wpepping         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Logger.hpp"
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
