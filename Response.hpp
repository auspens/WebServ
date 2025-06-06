/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: auspensk <auspensk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 10:15:00 by auspensk          #+#    #+#             */
/*   Updated: 2025/06/06 18:31:35 by auspensk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Source/StaticFileSource.hpp"
#include "Source/RedirectSource.hpp"
#include "Source/CGISource.hpp"

#define PROTOCOL "HTTP/1.1"

class Response {
	public:
		Response();
		Response (const Source *source);
		~Response();
		Response (const Response &src);
		Response &operator=(const Response &other);
		// static std::map <int, std::string>	_statusCodesAndTexts;

		const char *getHeader()const;
		bool isChunked()const;
		size_t getOffset()const;
		bool headerSent()const;
		void setHeaderSent(bool sent);
		void setOffset(ssize_t bytes_sent);

	private:
		std::string					_header;
		bool						_chunked;
		static std::string num_to_str(size_t num);
		size_t						_offset;
		bool						_headerSent;
};
