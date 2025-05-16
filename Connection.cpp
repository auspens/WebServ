/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Connection.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wpepping <wpepping@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 16:46:34 by auspensk          #+#    #+#             */
/*   Updated: 2025/05/16 14:18:48 by wpepping         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Connection.hpp"

Connection::Connection() {
	_source = NULL;
	_response = NULL;
}

Connection::Connection(int fd) : _socket(fd) {
	_source = NULL;
	_response = NULL;
 }

Connection::Connection(int fd, struct addrinfo *addrinfo)
		: _socket(fd, addrinfo) { }

Connection::Connection(const Connection &src) {
	(void)src;
}

Connection::~Connection() {
	delete(_source);
	delete(_response);
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

void Connection::setResponse(const Source *source) {
	if (_response)
		delete(_response);
	_response = new Response(source);
}

void Connection::readFromSocket() {
	std::vector<char> buffer;
	buffer.reserve(READ_BUFFER);
	int valread = read(_socket.getFd(), buffer.data(), READ_BUFFER);
	if (_parser.parse(buffer.data(), valread) != RequestParser::COMPLETE)
		return ;
	_request = _parser.getRequest(); // maybe weird for it to sit here..
}

void Connection::writeToSocket() {
	if (!_response->headerSent())
		sendHeader();
	else if (_source->getType() != REDIRECT)
		sendFromSource();
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

void Connection::sendHeader(){
	const char *buf = _response->getHeader() + _response->getOffset();
	ssize_t size = std::strlen(buf) > READ_BUFFER ? READ_BUFFER : std::strlen(buf);
	ssize_t bytes_sent = send(_socket.getFd(), buf, size, 0);
	if (bytes_sent == -1)
		throw (std::runtime_error("Error sending header"));
	if (bytes_sent >= (ssize_t)std::strlen(buf))
		_response->setHeaderSent(true);
	_response->setOffset(bytes_sent);
}

void Connection::sendFromSource(){
	if (_source->_bytesToSend < 1)
		return ;
	const char *buf = _source->getBytesRead().data() + _source->_offset;
	ssize_t size = _source->_bytesToSend > READ_BUFFER ? READ_BUFFER : _source->_bytesToSend;
	ssize_t bytes_sent = send(_socket.getFd(), buf, size, 0);
	if (bytes_sent == -1)
		throw (std::runtime_error("Error sending body"));
	_source->_bytesToSend -= bytes_sent;
	_source->_offset += bytes_sent;
}
