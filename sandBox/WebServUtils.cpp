/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WebServUtils.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wouter <wouter@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 17:15:00 by wouter            #+#    #+#             */
/*   Updated: 2025/04/26 17:16:31 by wouter           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "WebServUtils.hpp"

void WebServUtils::printErrorAndExit(std::string message, int exit_status) {
	std::cout << message << std::endl;
	std::exit(exit_status);
}
