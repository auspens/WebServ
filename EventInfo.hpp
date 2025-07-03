/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   EventInfo.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wouter <wouter@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 18:39:53 by wouter            #+#    #+#             */
/*   Updated: 2025/07/03 19:00:52 by wouter           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

class Connection;
class ListeningSocket;

enum EpollEventType {
	LISTENING_SOCKET,
	SOCKET,
	SOURCE
};

struct EventInfo {
	EpollEventType type;
	union {
		Connection		*conn;
		ListeningSocket	*listeningSocket;
	};

	EventInfo(EpollEventType type, void *ptr) : type(type) {
		if (type == LISTENING_SOCKET)
			this->listeningSocket = static_cast<ListeningSocket*>(ptr);
		else
			this->conn = static_cast<Connection*>(ptr);
	}
};
