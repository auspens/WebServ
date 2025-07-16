
#include "Connection.hpp"

Connection::Connection() { }

Connection::Connection(int fd, int serverPort) :
	_socket(fd),
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
	_source(NULL),
	_serverPort(serverPort),
	_invalidated(false),
	_lastActiveTime(std::time(0)),
	_sourceEventInfo(new EventInfo(SOURCE, this)),
	_socketEventInfo(new EventInfo(SOCKET, this)) {
		Logger::debug() << "Create new connection with fd: " << fd << std::endl;
}

Connection::Connection(const Connection &src):
	_socket(src._socket)
	,_parser(src._parser)
	,_request(src._request)
	,_source(src._source)
	,_serverConfig(src._serverConfig)
	,_location(src._location)
	,_serverPort(src._serverPort)
	,_invalidated(src._invalidated)
	,_lastActiveTime(src._lastActiveTime)
	,_sourceEventInfo(src._sourceEventInfo) {}

Connection::~Connection() {
	if (_source){
		delete _source ;
		_source = NULL;
	}
	if (_sourceEventInfo){
		delete _sourceEventInfo;
		_sourceEventInfo = NULL;
	}
	if (_socketEventInfo){
		delete _socketEventInfo;
		_socketEventInfo = NULL;
	}
}

Connection &Connection::operator=(const Connection &other) {
	if (this != &other){
		_socket = other._socket;
		_parser = other._parser;
		_request = other._request;
		_source = other._source;
		_serverConfig = other._serverConfig;
		_location = other._location;
		_serverPort = other._serverPort;
		_invalidated = other._invalidated;
		_lastActiveTime = other._lastActiveTime;
		_sourceEventInfo = other._sourceEventInfo;
	}
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

std::string Connection::getRequestBody() const{
	return _request.body;
}

void Connection::setupSource(bool &shutDownFlag) throw(SourceAndRequestException, IsChildProcessException) {
	if (_source){
		delete(_source);
		_source = NULL;
	}
	_source = SourceFactory::getNewSource(*_serverConfig, _location, _request, shutDownFlag);
	_source->init();
}

void Connection::setupErrorPageSource(const Config &config, int code) throw() {
	if (_source){
		delete(_source);
		_source = NULL;
	}
	_serverConfig = _findServerConfig(_serverPort, _request.hostname, config);
	_source = SourceFactory::getNewErrorPageSource(*_serverConfig, _location, _request, code);
	_source->init();
}

void Connection::readFromSocket(size_t bufferSize, const Config *config)
	throw(SourceAndRequestException, EmptyRequestException, SocketException) {
	std::vector<char> buffer;

	buffer.reserve(bufferSize);

	int valread = read(_socket.getFd(), buffer.data(), bufferSize);
	if (valread == -1)
		throw SocketException(std::string("Error reading from socket") + strerror(errno));
		
	if (_parser.getParseState() == RequestParser::START_LINE)
		_parser.initMaxBody(*config);
	RequestParser::ParseResult parseResult = _parser.parse(buffer.data(), valread);
	if (parseResult == RequestParser::URL_READY) {
		_serverConfig = _findServerConfig(_serverPort,_request.hostname, *config);
		Logger::debug() << "Request path in connection: " << _parser.getRequest().path << std::endl;
		_location = _findLocation(_parser.getRequest().path, *_serverConfig);
		_parser.setMaxBody(Config::getClientMaxBodySize(*_serverConfig, _location));
		Logger::debug() << "MaxBody is set to: " << _parser.getMaxBody() << std::endl;
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

	_setLastSocketActiveTime(std::time(0));
}

void Connection::writeToSocket() throw(SocketException) {
	const char	*buf = _source->readFromBuffer();

	if (_source->_bytesToSend > 0) {
		ssize_t 	size = std::min(_source->_bytesToSend, _serverConfig->getBufferSize());
		ssize_t		bytes_sent;

		Logger::debug() << ">> Sending to socket. Bytes to send: " << _source->_bytesToSend << std::endl;
		Logger::debug() << "Sending buffer: " << std::endl << std::string(buf).substr(0, _source->_bytesToSend) << std::endl;

		bytes_sent = send(_socket.getFd(), buf, size, 0);
		if (bytes_sent == -1)
			throw SocketException(std::string("Error sending to socket: ") + strerror(errno));

		Logger::debug() << "Sent " << bytes_sent << " bytes:" << std::endl;
		_source->bytesSent(bytes_sent);
	}

	_setLastSocketActiveTime(std::time(0));
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

void Connection::finishRequest() {
	_parser.reset();
	delete _source;
	_source = NULL;
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

bool Connection::isInvalidated() const {
	return _invalidated;
}

bool Connection::checkSocketTimeout(int timeout) const {
	return std::time(0) - getLastActiveTime() > timeout;
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

void Connection::_setLastSocketActiveTime(time_t time) {
	_lastActiveTime = time;
}

Connection::SocketException::SocketException() : message("Socket error") { }

Connection::SocketException::SocketException(const std::string &msg) :
	message(msg) { }

const char* Connection::SocketException::what() const throw() {
	return message.c_str();
}

Connection::SocketException::~SocketException() throw() { }
