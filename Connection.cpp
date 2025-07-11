/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Connection.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wpepping <wpepping@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 16:46:34 by auspensk          #+#    #+#             */
/*   Updated: 2025/07/11 18:37:06 by wpepping         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Connection.hpp"

Connection::Connection() { }

Connection::Connection(int fd, int serverPort) :
	_socket(fd),
	_response(NULL),
	_source(NULL),
	_serverPort(serverPort),
	_invalidated(false),
	_lastActiveTime(std::time(0)),
	_sourceEventInfo(new EventInfo(SOURCE, this)),
	_socketEventInfo(new EventInfo(SOCKET, this)) {
		Logger::debug() << "Create new connection with fd: " << fd << std::endl;
 }

Connection::Connection(int fd, int serverPort, struct addrinfo *addrinfo) :
	_socket(fd, addrinfo),
	_response(NULL),
	_source(NULL),
	_serverPort(serverPort),
	_invalidated(false),
	_lastActiveTime(std::time(0)),
	_sourceEventInfo(new EventInfo(SOURCE, this)),
	_socketEventInfo(new EventInfo(SOCKET, this)) {
		Logger::debug() << "Create new connection with fd: " << fd << std::endl;
}

Connection::Connection(const Connection &src) {
	(void)src;
}

Connection::~Connection() {
	delete _source ;
	delete _response;
	delete _sourceEventInfo;
	delete _socketEventInfo;
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

HttpRequest Connection::getRequest() const {
	return _request;
}

std::string Connection::getRequestBody()const{
	return _request.body;
}

void Connection::setupSource(bool &shutDownFlag) throw(SourceAndRequestException, IsChildProcessException) {
	if (_source)
		delete(_source);

	_source = SourceFactory::getNewSource(*_serverConfig, _location, _request, shutDownFlag);
	_source->init();
}

void Connection::setupErrorPageSource(const Config &config, int code) throw() {
	if (_source)
		delete(_source);

	_serverConfig = _findServerConfig(_serverPort, _request.hostname, config);
	_source = SourceFactory::getNewErrorPageSource(*_serverConfig, _location, _request, code);
	_source->init();
}

void Connection::setResponse() {
	if (_response)
		delete(_response);
	_response = new Response(_source);
	_source->setHeader(_response->getHeader());
}

void Connection::readFromSocket(size_t bufferSize, const Config *config)
	throw(SourceAndRequestException, EmptyRequestException) {
	std::vector<char> buffer;

	buffer.reserve(bufferSize);
	int valread = read(_socket.getFd(), buffer.data(), bufferSize);

	RequestParser::ParseResult parseResult = _parser.parse(buffer.data(), valread);
	if (parseResult == RequestParser::URL_READY) {
		_serverConfig = _findServerConfig(_serverPort,_request.hostname, *config);
		std::cout << "Request path: " << _request.path << std::endl;
		_location = _findLocation(_parser.getRequest().path, *_serverConfig);
		_parser.setMaxBody(Config::getClientMaxBodySize(*_serverConfig, _location));
		parseResult = _parser.continueParsing();
	}

	if (parseResult == RequestParser::EMPTY)
		throw(EmptyRequestException());
	else if (parseResult == RequestParser::COMPLETE) {
		_request = _parser.getRequest();

		Logger::debug() << "Headers:" << std::endl;
		for (std::map<std::string, std::string>::iterator it = _request.headers.begin(); it != _request.headers.end(); ++it)
			Logger::debug() << it->first << " : " << it->second << std::endl;
		Logger::debug() << std::endl;
	}
}

void Connection::writeToSocket() {
	const char	*buf = _source->readFromBuffer();

	if (_source->_bytesToSend > 0) {
		ssize_t 	size = std::min(_source->_bytesToSend, _serverConfig->getBufferSize());
		ssize_t		bytes_sent;

		Logger::debug() << ">> Sending to socket. Source type: " << _source->getType() << " Bytes to send: " << _source->_bytesToSend << std::endl;
		Logger::debug() << "Sending buffer: " << std::endl << std::string(buf).substr(0, _source->_bytesToSend) << std::endl;

		bytes_sent = send(_socket.getFd(), buf, size, 0);
		if (bytes_sent == -1)
			throw std::runtime_error(std::string("Error sending body: ") + strerror(errno)); // This should probably be a different type of exception. Also needs to be caught in Server or program will crash

		Logger::debug() << "Sent " << bytes_sent << " bytes:" << std::endl;

		_source->_bytesToSend -= bytes_sent; // Use setter
		_source->_offset += bytes_sent; // Use setter
	}
}

bool Connection::doneReadingSource() const {
	return (_source->_doneReading);
}

bool Connection::doneWritingSource() const {
	return (_source->_doneWriting);
}

bool Connection::doneWritingSocket() const {
	return getSource()->_bytesToSend == 0;
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
				return serverConfig;

			for (size_t i = 0; i < serverConfig->getServerNames().size(); i++) {
				if (_matchServerName(host, serverConfig->getServerNames()[i]))
					return serverConfig;
			}
		}
	}
	return NULL;
}

const Location *Connection::_findLocation (
	const std::string &target,
	const ServerConfig &serverConfig
) {
	const std::vector<Location *> locations = serverConfig.getLocations();
	std::vector<Location *>::const_iterator it;
	for (it = locations.begin(); it != locations.end(); ++it) {
		std::string locationPath = (*it)->getPath();

		if (target.compare(0, locationPath.size(), locationPath) == 0
			&& (target.size() == locationPath.size()
			|| WebServUtils::isin("/#?", target.at(locationPath.size()))))
			return *it;
	}
	return (NULL);
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

void Connection::invalidate() {
	_invalidated = true;
}

int Connection::isInvalidated() const {
	return _invalidated;
}

time_t Connection::getLastActiveTime() const {
	return _lastActiveTime;
}


EventInfo *Connection::getSourceEventInfo() const {
	return _sourceEventInfo;
}

EventInfo *Connection::getSocketEventInfo() const {
	return _socketEventInfo;
}

void Connection::setLastActiveTime(time_t time) {
	_lastActiveTime = time;
}
