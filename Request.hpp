/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wouter <wouter@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 14:09:37 by wouter            #+#    #+#             */
/*   Updated: 2025/04/26 16:42:02 by wouter           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string>

class Request {
public:
	Request();
	~Request();
	bool	isReady() const;
	void	read(int fd, int buffer_size);

private:
	std::string	_requestType;
	std::string	_target;
	bool		_isReady;
};
