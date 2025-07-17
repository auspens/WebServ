/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SystemCallsUtilities.cpp                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wpepping <wpepping@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 16:52:55 by wouter            #+#    #+#             */
/*   Updated: 2025/07/02 18:43:13 by wpepping         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "SystemCallsUtilities.hpp"

SystemCallsUtilities::SystemCallsUtilities() {}
SystemCallsUtilities::~SystemCallsUtilities() {}

void SystemCallsUtilities::check_for_error(int return_value, const std::string message){
	if (return_value < 0){
		Logger::error() << message << ": " << strerror(errno) << std::endl;
		std::exit(EXIT_FAILURE);
	}
}

