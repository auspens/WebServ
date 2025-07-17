/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   EventInfo.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wpepping <wpepping@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 18:39:53 by wouter            #+#    #+#             */
/*   Updated: 2025/07/11 16:12:41 by wpepping         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

class Connection;
class ListeningSocket;

enum EpollEventType {
	LISTENING_SOCKET,
	SOCKET,
	SOURCE,
	CHILD
};

struct EventInfo {
	EpollEventType type;
	union {
		Connection		*conn;
		ListeningSocket	*listeningSocket;
		int				fd;
	};

	EventInfo(EpollEventType type, void *ptr) : type(type) {
		if (type == LISTENING_SOCKET)
			this->listeningSocket = static_cast<ListeningSocket*>(ptr);
		else
			this->conn = static_cast<Connection*>(ptr);
	}
};
