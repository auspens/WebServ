/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   UploadSource.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wouter <wouter@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/29 15:44:12 by wpepping          #+#    #+#             */
/*   Updated: 2025/07/06 19:08:52 by wouter           ###   ########.fr       */
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
			HttpRequest &req
		);
		~UploadSource();
		void 	readSource();
		void 	writeSource();


	private:
		std::vector <fileToUpload> _uploads;
		bool _isWriting;
		ssize_t _writeSize;
		size_t _uploadOffset;

		void _getUploadFiles(std::string boundary, HttpRequest &req);
		std::string _getFileName(std::string token);
		void _createHTTPResponse();
		std::string _findBoundary(std::string header);
};
