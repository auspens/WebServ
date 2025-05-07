/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Connection.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: auspensk <auspensk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 16:46:34 by auspensk          #+#    #+#             */
/*   Updated: 2025/05/07 17:13:15 by auspensk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Connection.hpp"

Connection::Connection() {
	_source = NULL;
}

Connection::Connection(int fd) : _socket(fd) {
	_source = NULL;
 }

Connection::Connection(int fd, struct addrinfo *addrinfo)
		: _socket(fd, addrinfo) { }

Connection::Connection(const Connection &src) {
	(void)src;
}

Connection::~Connection() {
	delete(_source);
}

Connection &Connection::operator=(const Connection &other) {
	(void)other;
	return *this;
};

int Connection::getSocketFd() const {
	return _socket.getFd();
}

int Connection::getSourceFd() const {
	return _source->getFd();
}

Source *Connection::getSource()const{
	return _source;
}

void Connection::setSource(Source *source){
	if (_source)
		delete(_source);
	_source = source;
}


static std::string num_to_str(size_t num) {
	std::ostringstream convert;   // stream used for the conversion
	convert << num;      // insert the textual representation of 'Number' in the characters in the stream
	return convert.str();
}

void Connection::readFromSocket(int buffer_size) {

	std::vector<char> buffer;
	buffer.reserve(buffer_size);
  	int valread = read(_socket.getFd(), buffer.data(), buffer_size);
	if (_parser.parse(buffer.data(), valread) != RequestParser::COMPLETE)
		return ;
    _request = _parser.getRequest(); // maybe weird for it to sit here..
	prepSource();

}

void Connection::generateResponseHeaders() {
	_response.responseHeaders += _response.http_version + " " + _response.code + " " + _response.status + "\r\n";
	for (std::map<std::string, std::string>::iterator it = _response.headers.begin(); it != _response.headers.end(); ++it)
		_response.responseHeaders += it->first + ": " + it->second + "\r\n";
	_response.responseHeaders += "\r\n"; // END of headers: blank line
}

void Connection::writeToSocket(int buffer_size) {
	(void)buffer_size;

	if (!headerSent())
		sendHeader();
	else
		sendFromSource();

	ssize_t bytes_sent = send(_socket.getFd(), response.c_str(), response.length(), 0);
	std::cout << "Bytes sent in the responce: " << bytes_sent << std::endl;
	if (bytes_sent != (ssize_t)response.length()) {
		std::cerr << "Partial write! Only sent " << bytes_sent << " bytes out of " << response.length() << std::endl;
		//  must handle it here (loop, or error)
	}

	// Tell the peer that we finished sending
  	//shutdown(_socket.getFd(), SHUT_RDWR);

	_socket.close_sock();
}

void Connection::prepSource() {
}

void Connection::generateResponse() {

	_response.headers.clear();
	_response.body = "";
	_response.http_version = _request.http_version;

	if (_request.uri.substr(0,6) == "/echo/") {
		_response.body = _request.uri.substr(6);
		_response.code = "200";
		_response.status = "OK";
		_response.headers["Content-Type"] = "text/plain";
		_response.headers["Content-Length"] = num_to_str(_response.body.length());

	}
	else if (_request.uri.substr(0,11) == "/user-agent") {
		_response.body = _request.headers["User-Agent"];
		_response.code = "200";
		_response.status = "OK";
		_response.headers["Content-Type"] = "text/plain";
		_response.headers["Content-Length"] = num_to_str(_response.body.length());
	}
	else if (_request.uri.substr(0,11) == "/heavy.jpg") {
		_response.code = "200";
		_response.status = "OK";
		_response.headers["Content-Type"] = "image/jpeg";
		_response.headers["Transfer-Encoding"] = "chunked";
	}
	else {
		_response.code = num_to_str(_source->getCode());
		_response.status = _response.code == "200"? "OK" : "BAD";
		//_response.body = std::string(_source->getBytesRead().begin(), _source->getBytesRead().end());
		_response.headers["Content-Length"] = num_to_str(_source->getSize());
		_source->_bytesToSend -= _source->getSize();//this should subtract the actual size of the chunk sent
	}
}

bool Connection::requestReady() const {
	return _parser.isDone();
}

void Connection::resetParser() {
	_parser.reset();
}

const std::string& Connection::getTarget() const {
	return (_request.uri);
}
