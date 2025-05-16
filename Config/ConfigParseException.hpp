/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigParseException.hpp                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wouter <wouter@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/09 16:03:39 by wpepping          #+#    #+#             */
/*   Updated: 2025/05/14 17:50:27 by wouter           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <string>

class ConfigParseException : public std::exception {
public:
	ConfigParseException(const std::string &error);
	~ConfigParseException() throw();
	virtual const char* what() const throw();

private:
	std::string _error;
};
