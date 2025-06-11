/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   UploadSource.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: auspensk <auspensk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/29 15:44:12 by wpepping          #+#    #+#             */
/*   Updated: 2025/06/11 15:16:31 by auspensk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string>
#include "Source.hpp"

class UploadSource : Source {
	public:
		UploadSource(
			const ServerConfig &serverConfig,
			Location const &location,
			HttpRequest req
		);
		~UploadSource();

	private:
		std::map <std::string, std::string> _uploads;
		std::string _filename;
};
