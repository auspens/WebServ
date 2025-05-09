/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigParseException.hpp                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wpepping <wpepping@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/09 16:03:39 by wpepping          #+#    #+#             */
/*   Updated: 2025/05/09 16:03:43 by wpepping         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <string>

class ConfigParseException : std::exception {
public:
	ConfigParseException(const std::string &error);
	virtual const char* what() const throw();

private:
	std::string _error;
};
