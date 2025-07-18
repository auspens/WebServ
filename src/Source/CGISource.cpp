#include "CGISource.hpp"

std::map<pid_t, int> CGISource::outputPipeWriteEnd;
std::map<pid_t, int> CGISource::exitStatus;

CGISource::CGISource(
	const ServerConfig &serverConfig,
	Location const *location,
	HttpRequest &req,
	std::string target
) : Source(serverConfig, location, req) {
	Logger::debug() << "Creating CGI Source" << std::endl;

	_childPid = 0;
	_target = target;

	_extension = WebServUtils::getCgiExtension(_target, Config::getAcceptCgi(serverConfig, location));
	size_t script_end = _target.find(_extension) + _extension.length();
	if (script_end != std::string::npos) {
		_pathInfo = _target.substr(script_end);
		_target = _target.substr(0, script_end);
	}

	_scriptName = _target.substr(Config::getRootFolder(_serverConfig, _location).length());

	if (_checkIfExists())
		_forkAndExec();
	else
		throw SourceAndRequestException("Url not found", 404);
}

void CGISource::init() throw(SourceAndRequestException) {
	Source::init();

	_childExited = false;
	_childLastActive = std::time(0);
	_pollableRead = true;
	_pollableWrite = true;
	_writeWhenComplete = true;
	_fd = _outputPipe[0];
	_writeFd = _inputPipe[1];

	_writeOffset = 0;
	_readBuffer.resize(_serverConfig.getBufferSize());
	_body.resize(0);

	if (_request.method == "POST")
		_doneWriting = false;
}

CGISource::~CGISource() {
	std::map<pid_t, int>::iterator	it;

	Logger::debug() << "CGISource destructor called" << std::endl;

	close(_fd);
	close(_writeFd);
	if (_childPid > 0 && !_childExited) {
		kill(_childPid, SIGTERM);
		kill(_childPid, SIGKILL);
	}

	it = CGISource::exitStatus.find(_childPid);
	if (it != exitStatus.end())
		CGISource::exitStatus.erase(it);
}

void CGISource::setHeader() {
	std::string					header;

	header += "HTTP/1.1 200 OK\r\n";
	if (_request.isNotKeepAlive())
		header += "Connection: Keep-Alive\r\n";
	header += "Content-Length: " + WebServUtils::num_to_str(_getContentLength()) + "\r\n";
	Logger::debug()<< "At setHeader" << std::endl;
	Logger::debug()<< "Header: " << std::endl << header << "header length: "<< header.length() << std::endl;
	_body.insert(_body.begin(), header.begin(), header.end());
	_bytesToSend += header.length();
	Logger::debug() << "Bytes to send: " << _bytesToSend << std::endl;
}

size_t CGISource::_getContentLength() const {
	std::vector<char>::const_iterator	it;
	std::string							needle = "\r\n\r\n";
	std::string							needle2 = "\n\n";
	int									needle_len = 4;

	it = std::search(_body.begin(), _body.end(), needle.begin(), needle.end());
	if (it == _body.end()) {
		it = std::search(_body.begin(), _body.end(), needle2.begin(), needle2.end());
		needle_len -= 2;
	}

	return (it == _body.end() ? 0 : _bytesToSend - (it - _body.begin()) - needle_len);
}

void CGISource::_forkAndExec() throw(IsChildProcessException) {
	Logger::debug() << "in forkAndExec()" << std::endl;
	std::vector<std::string>	argv;
	std::vector<std::string>	envp;
	pid_t						pid;

	pipe(_inputPipe);
	pipe(_outputPipe);

	pid = fork();
	if (pid < 0) {
		perror("fork");
	} else if (pid == 0) { //CHILD
		close(_inputPipe[1]);
		close(_outputPipe[0]);
		_buildArgv(argv);
		_buildEnvp(envp);
		Logger::debug() << "Child: Throwing IsChildProcessException" << std::endl;
		throw IsChildProcessException(argv, envp, _inputPipe[0], _outputPipe[1]);
	} else { // PARENT
		Logger::debug() << "Closing child pipe ends in parent" << std::endl;
		_childPid = pid;
		Logger::debug() << "_childPid in parent " << _childPid << std::endl;
		close(_inputPipe[0]);
		CGISource::outputPipeWriteEnd[pid] = _outputPipe[1]; // will be closed in signal handler
	}
}

void CGISource::_buildArgv(std::vector<std::string> &argv) {
	argv.push_back(Config::getAcceptCgi(_serverConfig, _location).at(_extension));
	argv.push_back(_target);
}

void CGISource::_buildEnvp(std::vector<std::string> &envp) {
	size_t qmark = _request.uri.find('?');

	_queryString = (qmark == std::string::npos ? "" : _request.uri.substr(qmark + 1));

	if (_pathInfo.length())
		envp.push_back(std::string("PATH_INFO=") + _pathInfo);
	envp.push_back(std::string("REQUEST_METHOD=") + _request.method);
	envp.push_back(std::string("QUERY_STRING=") + _queryString);
	envp.push_back(std::string("SCRIPT_NAME=") + _scriptName);
	envp.push_back(std::string("SCRIPT_FILENAME=") + _target);
	envp.push_back("SERVER_PROTOCOL=HTTP/1.1");

	_addEnvVarFromHeader(envp, _request.headers, "CONTENT_LENGTH", "Content-Length");
	_addEnvVarFromHeader(envp, _request.headers, "CONTENT_TYPE", "Content-Type");
	_addEnvVarFromHeader(envp, _request.headers, "HTTP_COOKIE", "Cookie");
	envp.push_back(std::string("REDIRECT_STATUS=200"));
}

void CGISource::_addEnvVarFromHeader(
	std::vector<std::string> &envp,
	const std::map<std::string, std::string> &headers,
	const std::string &envVar,
	const std::string &header
) {
	std::map<std::string, std::string>::const_iterator it = headers.find(header);
	if (it != headers.end()) {
		envp.push_back(envVar + "=" + it->second);
	}
}

bool CGISource::getIfExists() const {
	return (_pathExists);
}

bool CGISource::_checkIfExists(){
	DIR *dir = opendir(_serverConfig.getRootFolder().c_str());
	if (!dir)
		return (0);
	closedir(dir);
	if (access(_target.c_str(), R_OK) == -1)
		return (0);
	Logger::debug() << _target << " exists" << std::endl;
	return (1);
}

void CGISource::readSource() throw(SourceAndRequestException) {
	if (!_childProcessHealthy())
		throw SourceAndRequestException("Child process returned error", 500);
	if (!_doneReading) {
		Logger::debug() << "Bytes to send: " << _bytesToSend << std::endl;

		long long bytesRead = read(_outputPipe[0], _readBuffer.data(), _serverConfig.getBufferSize());
		if (bytesRead < 0)
			throw SourceAndRequestException("Child process returned error", 500);
		if (bytesRead == 0) {
			_doneReading = true;
			setHeader();
		} else
			_body.insert(_body.end(), _readBuffer.begin(), _readBuffer.begin() + bytesRead);

		_bytesToSend += bytesRead;
		_size += bytesRead;

		Logger::debug() << "Read " << bytesRead << " bytes from cgi source: " << std::endl << std::endl;
		Logger::debug() << "Bytes to send: " << _bytesToSend << std::endl;
		if (bytesRead > 0) Logger::detail() << "Data read from source: "
			<< std::endl << std::string(_readBuffer.data(), bytesRead) << std::endl;
		if (bytesRead > 0) Logger::detail() << "Body data (size=" << _body.size() << "): "
			<< std::endl << std::string(_body.data(), _bytesToSend) << std::endl;
		if (_doneReading) Logger::debug() << "Done reading CGI source" << std::endl;
	}

	_childLastActive = std::time(0);
}

void CGISource::writeSource() throw(SourceAndRequestException) {
	long	numbytes;
	size_t	unsigned_numbytes;
	size_t	writeSize = std::min(Config::getClientMaxBodySize(_serverConfig, _location), _request.body.length() - _writeOffset);

	numbytes = write(_writeFd, _request.body.c_str() + _writeOffset, writeSize);
	if (numbytes == -1)
		throw SourceAndRequestException("Error writing to CGI process", 500);

	unsigned_numbytes = static_cast<size_t>(numbytes);
	_writeOffset += unsigned_numbytes;
	if (_writeOffset == unsigned_numbytes) {
		Logger::debug() << "Done writing to cgi, closing write end of input pipe: fd " << _writeFd << std::endl;
		_doneWriting = true;
	}

	_childLastActive = std::time(0);
}

bool CGISource::_childProcessHealthy() {
	std::map<pid_t, int>::iterator	it;
	int								status;

	it = CGISource::exitStatus.find(_childPid);
	if (it == exitStatus.end())
		return true;

	_childExited = true;
	status = it->second;
	CGISource::exitStatus.erase(it);
	if (status == 0)
		return true;
	return false;
}

bool CGISource::checkTimeout(int timeout) const {
	if ((_doneReading == false || _doneWriting == false)
		&& std::time(0) - _childLastActive > timeout)
			return true;
	return false;
}

CGISource::CGISource(const CGISource &other) : Source(other){
	_pathExists = other._pathExists;
	_outputPipe[0] = other._outputPipe[0];
	_outputPipe[1] = other._outputPipe[1];
	_inputPipe[0] = other._inputPipe[0];
	_inputPipe[1] = other._inputPipe[1];
	_readBuffer = other._readBuffer;
	_queryString = other._queryString;
	_pathInfo = other._pathInfo;
	_scriptName = other._scriptName;
	_writeOffset = other._writeOffset;
	_childPid = other._childPid;
	_childLastActive = other._childLastActive;
	_extension = other._extension;
}

CGISource &CGISource:: operator=(const CGISource &other){
	if (this != &other){
		Source::operator=(other);
		_pathExists = other._pathExists;
		_outputPipe[0] = other._outputPipe[0];
		_outputPipe[1] = other._outputPipe[1];
		_inputPipe[0] = other._inputPipe[0];
		_inputPipe[1] = other._inputPipe[1];
		_readBuffer = other._readBuffer;
		_queryString = other._queryString;
		_pathInfo = other._pathInfo;
		_scriptName = other._scriptName;
		_writeOffset = other._writeOffset;
		_childPid = other._childPid;
		_childLastActive = other._childLastActive;
		_extension = other._extension;
	}
	return *this;
}

void CGISource::finalizeWrite() {
	close(_writeFd);
}
