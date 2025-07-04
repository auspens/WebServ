/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SourceAndRequestException.hpp                      :+:      :+:    :+:   */
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

class SourceAndRequestException : std::exception{
	private:
		std::string _error;
		int			_code;

	public:
		SourceAndRequestException(std::string error, int code) throw();
		const char *what() const throw();
		int errorCode() const throw();
		~SourceAndRequestException()throw();
};
