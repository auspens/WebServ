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

	_childLastActive = std::time(0);
	_pollableRead = true;
	_pollableWrite = true;
	_writeWhenComplete = true;
	_fd = _outputPipe[0];
	_writeFd = _inputPipe[1];

	_writeOffset = 0;
	_readBuffer.resize(_serverConfig.getBufferSize());

	if (_request.method == "POST")
		_doneWriting = false;
}

CGISource::~CGISource() {
	Logger::debug() << "CGISource destructor called" << std::endl;
	close(_fd);
	close(_writeFd);
}

void CGISource::setHeader() {
	std::string					header;

	header += "HTTP/1.1 200 OK\r\n";
	if (_request.isKeepAlive())
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
		_buildArgv(argv);
		_buildEnvp(envp);
		close(_inputPipe[1]);
		close(_outputPipe[0]);
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

	envp.push_back(std::string("CONTENT_LENGTH=") + _request.headers["Content-Length"]);
	envp.push_back(std::string("CONTENT_TYPE=") + _request.headers["Content-Type"]);
	envp.push_back(std::string("REDIRECT_STATUS=200"));
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
		size_t bytesRead = read(_outputPipe[0], _readBuffer.data(), _serverConfig.getBufferSize());
		_body.insert(_body.end(), _readBuffer.begin(), _readBuffer.begin() + bytesRead);

		if (bytesRead == 0) {
			_doneReading = true;
			setHeader();
		}

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

void CGISource::writeSource() {
	size_t numbytes;
	size_t writeSize = std::min(Config::getClientMaxBodySize(_serverConfig, _location), _request.body.length() - _writeOffset);

	numbytes = write(_writeFd, _request.body.c_str() + _writeOffset, writeSize);
	_writeOffset += numbytes;
	if (_writeOffset == numbytes) {
		Logger::debug() << "Closing write end of input pipe: Closing fd " << _writeFd << std::endl;
		_doneWriting = true;
		close(_writeFd);
	}

	_childLastActive = std::time(0);
}

bool CGISource::_childProcessHealthy() const {
	std::map<pid_t, int>::iterator	it;
	int								status;

	it = CGISource::exitStatus.find(_childPid);
	if (it == exitStatus.end())
		return true;

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
