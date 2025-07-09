/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ShutDownRequestException.hpp                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wpepping <wpepping@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 14:09:57 by wouter            #+#    #+#             */
/*   Updated: 2025/07/04 15:55:42 by wpepping         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <string>
#include <exception>

class ShutDownRequestException : std::exception{
	private:
		std::string _error;

	public:
		ShutDownRequestException(std::string error) throw();
		const char *what() const throw();
		~ShutDownRequestException()throw();
};
