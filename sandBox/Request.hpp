/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wouter <wouter@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 14:09:37 by wouter            #+#    #+#             */
/*   Updated: 2025/04/25 14:11:24 by wouter           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string>

class Request {
public:
	Request();
	~Request();

private:
	std::string	_requestType;
	std::string _target;
	bool		_isReady
};
