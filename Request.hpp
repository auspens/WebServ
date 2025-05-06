/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: auspensk <auspensk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 14:09:37 by wouter            #+#    #+#             */
/*   Updated: 2025/04/29 10:27:09 by auspensk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string>

class Request {
public:
	Request();
	~Request();
	bool	isReady() const;
	void	read(int fd, int buffer_size);
	const std::string &getTarget() const;

private:
	std::string	_requestType;
	std::string	_target;
	bool		_isReady;
};
