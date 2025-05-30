/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   UploadSource.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wpepping <wpepping@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/29 15:44:12 by wpepping          #+#    #+#             */
/*   Updated: 2025/05/29 16:09:41 by wpepping         ###   ########.fr       */
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

		std::string _filename;
};
