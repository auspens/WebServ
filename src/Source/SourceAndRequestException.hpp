/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SourceAndRequestException.hpp                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wpepping <wpepping@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 14:09:57 by wouter            #+#    #+#             */
/*   Updated: 2025/07/17 17:57:54 by wpepping         ###   ########.fr       */
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
		SourceAndRequestException(const SourceAndRequestException &src);
		SourceAndRequestException& operator=(const SourceAndRequestException &src);
		~SourceAndRequestException()throw();

		const char *what() const throw();
		int errorCode() const throw();
};
