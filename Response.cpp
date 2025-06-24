/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: auspensk <auspensk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 10:33:18 by auspensk          #+#    #+#             */
/*   Updated: 2025/06/24 14:25:39 by auspensk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp" //I'm not sure if we need this class as it is now.

Response::Response()
	: _header("")
	, _chunked(false){}
Response::Response (const Source *source)
	: _header("")
	, _chunked(false){
	_header += std::string(PROTOCOL) + " " + num_to_str(source->getCode()) + " " + Source::_statusCodes.find(source->getCode())->second.message + "\r\n";
	switch(source->getType())
	{
		case STATIC:
			_header += "Content-Type: " + source->getMime() + "\r\n";
			_header += "Content-Length: " + num_to_str(source->getSize()) + "\r\n";
			break;
		case REDIRECT:
			_header += "Location: " + source->getRedirectLocation() + "\r\n";
			_header += "Content-Length: 0\r\n";
			break;
		case CGI:
			_header += "Connection: Keep-Alive\r\n";
			_header += "Keep-Alive: timeout=5, max=997";
			//_header += "Transfer-Encoding: chunked\r\n";
			//_chunked = true;
			break;
		case UPLOAD:
			_header = "";
			break;
	}
	_header += "\r\n"; // END of headers: blank line
}
Response::~Response(){}
Response::Response (const Response &src): _header(src._header){}
Response &Response::operator=(const Response &other){
	if (this != &other)
		_header = other._header;
	return *this;
}
const char *Response::getHeader()const{
	return (_header.c_str());
}

std::string Response::num_to_str(size_t num) {
	std::ostringstream convert;
	convert << num;
	return convert.str();
}
bool Response::isChunked()const{
	return _chunked;
}

