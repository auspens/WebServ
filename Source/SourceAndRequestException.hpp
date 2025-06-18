/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SourceAndRequestException.hpp                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: auspensk <auspensk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 14:09:57 by wouter            #+#    #+#             */
/*   Updated: 2025/06/18 16:03:43 by auspensk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <string>
#include <exception>

class SourceAndRequestException : std::exception{
				std::string _error;
				int			_code;
			public:
				SourceAndRequestException(std::string error, int code) throw();
				const char *what() const throw();
				int errorCode() const throw();
				~SourceAndRequestException()throw();
		};
