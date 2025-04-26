/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WebServUtils.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wouter <wouter@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 14:38:47 by wouter            #+#    #+#             */
/*   Updated: 2025/04/26 17:16:34 by wouter           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cstdlib>
#include <iostream>
#include <sstream>
#include <string>

class WebServUtils {
public:
	template <typename T>
	static std::string to_string(T src) {
		std::ostringstream result;
		result << src;
		return result.str();
	}

	void printErrorAndExit(std::string message, int exit_status);

private:
	WebServUtils();
	~WebServUtils();
};
