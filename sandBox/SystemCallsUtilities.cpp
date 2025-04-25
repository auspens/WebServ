/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SystemCallsUtilities.cpp                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wouter <wouter@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 16:52:55 by wouter            #+#    #+#             */
/*   Updated: 2025/04/25 16:52:57 by wouter           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "SystemCallsUtilities.hpp"

SystemCallsUtilities::SystemCallsUtilities() {}
SystemCallsUtilities::~SystemCallsUtilities() {}

void SystemCallsUtilities::check_for_error(int return_value){
	if (return_value < 0){
		std::cout << strerror(errno) << std::endl;
		std::exit(EXIT_FAILURE);
	}
}

