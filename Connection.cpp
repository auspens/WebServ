/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Connection.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wpepping <wpepping@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 16:46:34 by auspensk          #+#    #+#             */
/*   Updated: 2025/05/23 17:53:50 by wpepping         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Connection.hpp"

Connection::Connection() {
	_source = NULL;
	_response = NULL;
}

Connection::Connection(int fd, int serverPort) : _socket(fd), _serverPort(serverPort) {
	_source = NULL;
	_response = NULL;
 }

Connection::Connection(int fd, int serverPort, struct addrinfo *addrinfo)
		: _socket(fd, addrinfo), _serverPort(serverPort) { }

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

Source *Connection::getSource() const {
	return _source;
}

void Connection::setupSource(const Config &config) throw(Source::SourceException) {
	if (_source)
		delete(_source);

	_serverConfig = _findServerConfig(_serverPort, _request.hostname, config);
	if (!_serverConfig)
		throw Source::SourceException("No matching server found"); //Should this be a different exception type?
	_source = Source::getNewSource(_request.uri, *_serverConfig);
}

void Connection::setResponse() {
	if (_response)
		delete(_response);
	_response = new Response(_source);
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
	{
		std::cout << "Will be sending header now" << std::endl;
		sendHeader();
	}
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

void Connection::sendHeader() {
	const char *buf = _response->getHeader() + _response->getOffset();
	ssize_t size = std::strlen(buf) > READ_BUFFER ? READ_BUFFER : std::strlen(buf);
	ssize_t bytes_sent = send(_socket.getFd(), buf, size, 0);
	if (bytes_sent == -1)
		throw (std::runtime_error("Error sending header"));
	if (bytes_sent >= (ssize_t)std::strlen(buf))
		_response->setHeaderSent(true);
	_response->setOffset(bytes_sent);
}

void Connection::sendFromSource() {
	const char *buf = _source->readFromSource();
	if (_source->_bytesToSend < 1)
		return ;
	ssize_t size = _source->_bytesToSend > READ_BUFFER ? READ_BUFFER : _source->_bytesToSend;
	ssize_t bytes_sent = send(_socket.getFd(), buf, size, 0);
	if (bytes_sent == -1)
		throw (std::runtime_error("Error sending body"));
	_source->_bytesToSend -= bytes_sent;
	_source->_offset += bytes_sent;
}

const ServerConfig *Connection::_findServerConfig(
	int port,
	const std::string host,
	const Config &config
) const {
	ServerConfig *serverConfig;

	for (size_t i = 0; i < config.getServerConfigs().size(); i++) {
		serverConfig = config.getServerConfigs()[i];

		if (port == serverConfig->getPort()) {
			if (serverConfig->getServerNames().size() == 0)
			{
				std::cout << "getRootFolder: " << serverConfig->getRootFolder() << std::endl;
				return serverConfig;
			}

			for (size_t i = 0; i < serverConfig->getServerNames().size(); i++) {
				if (_matchServerName(host, serverConfig->getServerNames()[i]))
					return serverConfig;
			}
		}
	}

	return NULL;
}

bool Connection::_matchServerName(std::string host, std::string serverName) const {
	int		hostLength = host.length();
	int		nameLength = serverName.length();
	char	lastChar = serverName[serverName.length() - 1];

	if (serverName == "*")
		return true;
	if (serverName[0] == '*' && serverName[serverName.length() - 1] == '*')
		return host.find(serverName.substr(1, serverName.length() - 2)) != std::string::npos;
	if (serverName[0] == '*')
		return hostLength >= nameLength - 1
			&& host.substr(hostLength - nameLength) == serverName.substr(1);
	if (lastChar == '*')
		return hostLength >= nameLength - 1
			&& host.substr(0, nameLength) == serverName.substr(0, nameLength - 1);
	return host == serverName;
}

void Connection::close() {
	_socket.close_sock();
}
