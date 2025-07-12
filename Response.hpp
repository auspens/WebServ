/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wouter <wouter@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 10:15:00 by auspensk          #+#    #+#             */
/*   Updated: 2025/07/12 18:32:32 by wouter           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Source/StaticFileSource.hpp"
#include "Source/RedirectSource.hpp"
#include "Source/CGISource.hpp"

class Response {
	public:
		Response();
		Response (const Source *source);
		~Response();
		Response (const Response &src);
		Response &operator=(const Response &other);
		// static std::map <int, std::string>	_statusCodesAndTexts;

		const char	*getHeader() const;
		bool		isChunked() const;

	private:
		std::string num_to_str(size_t num);

		std::string					_header;
		bool						_chunked;
};
