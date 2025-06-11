/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SourceAndRequestException.hpp                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: auspensk <auspensk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 10:23:47 by auspensk          #+#    #+#             */
/*   Updated: 2025/06/11 10:25:09 by auspensk         ###   ########.fr       */
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
