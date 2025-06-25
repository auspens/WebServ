/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   UploadSource.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: auspensk <auspensk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/29 15:44:12 by wpepping          #+#    #+#             */
/*   Updated: 2025/06/25 13:25:30 by auspensk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string>
#include <fcntl.h>
#include <sys/types.h>
#include <dirent.h>
#include "Source.hpp"

struct fileToUpload{
	std::string body;
	std::string name;
	std::string extension;
};

class UploadSource : public Source {
	public:
		UploadSource(
			const ServerConfig &serverConfig,
			Location const *location,
			HttpRequest req
		);
		~UploadSource();
		void 	readSource();
		void 	writeSource();


	private:
		std::vector <fileToUpload> _uploads;
		bool _isWriting;
		size_t _writeSize;

		void _getUploadFiles(std::string boundary, HttpRequest req);
		std::string _getFileName(std::string token);
		void _createHTTPResponse();
		std::string _findBoundary(std::string header);
};
