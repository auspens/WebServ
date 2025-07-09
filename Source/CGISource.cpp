#include "CGISource.hpp"

CGISource::CGISource(const ServerConfig &serverConfig, Location const *location, HttpRequest &req)
 : Source(serverConfig, location, req) {
	Logger::debug() << "Creating CGI Source" << std::endl;
}

void CGISource::init() throw(SourceAndRequestException, ChildProcessNeededException, ShutDownRequestException) {
	Source::init();

	_pollableRead = true;
	_pollableWrite = true;
	_writeWhenComplete = true;
	_type = CGI;
	_scriptPath = _request.path;
	_uri = _request.uri;
	_writeOffset = 0;
	_readBuffer.resize(_serverConfig.getBufferSize());

	if (_request.method == "POST")
		_doneWriting = false;

	size_t script_end = _scriptPath.find(".py") + 3; // Needs to be based on config file
	if (script_end != std::string::npos) {
		_pathInfo = _scriptPath.substr(script_end);
		_scriptPath = _scriptPath.substr(0, script_end);
	}

	_scriptPath = _serverConfig.getRootFolder() + _scriptPath;

	if (checkIfExists())
		forkAndExec();
	else
		throw SourceAndRequestException("Url not found", 404);
}

CGISource::~CGISource(){
	Logger::debug() << "CGISource destructor called" << std::endl;
	close(_inputPipe[1]);
	close(_outputPipe[0]);
}

void CGISource::forkAndExec() throw(ChildProcessNeededException) {
	Logger::debug() << "in forkAndExec()" << std::endl;
	std::vector<std::string>	envp;
	pid_t						pid;

	_inputPipe.resize(2);
	_outputPipe.resize(2);
	pipe(_inputPipe.data());
	pipe(_outputPipe.data());

	pid = fork();
	if (pid < 0) {
		perror("fork");
	} else if (pid == 0) { //CHILD
		buildEnvironmentVariables(envp);
		close(_inputPipe[1]);
		close(_outputPipe[0]);
		Logger::debug() << "Child: Throwing ChildProcessNeededException" << std::endl;
		throw ChildProcessNeededException(_scriptPath, envp, _inputPipe[0], _outputPipe[1]);
	} else { // PARENT
		Logger::debug() << "Closing child pipe ends in parent" << std::endl;
		close(_inputPipe[0]);
		close(_outputPipe[1]);
		_fd = _outputPipe[0];
		_writeFd = _inputPipe[1];
	}
}

void CGISource::buildEnvironmentVariables(std::vector<std::string> &envp) {
	size_t qmark = _uri.find('?');

	if (qmark != std::string::npos) {
		_queryString = _uri.substr(qmark + 1);
	} else {
		_queryString = "";
	}

	if (_pathInfo.length())
		envp.push_back(std::string("PATH_INFO=") + _pathInfo);
	envp.push_back(std::string("REQUEST_METHOD=") + _request.method);
	envp.push_back(std::string("QUERY_STRING=") + _queryString);
	envp.push_back(std::string("SCRIPT_NAME=") + _scriptPath);
	envp.push_back("SERVER_PROTOCOL=HTTP/1.1");

	envp.push_back(std::string("CONTENT_LENGTH=") + _request.headers["Content-Length"]);
	envp.push_back(std::string("CONTENT_TYPE=") + _request.headers["Content-Type"]);
}

bool CGISource::getIfExists() const {
	return (_pathExists);
}

bool CGISource::checkIfExists(){
	DIR *dir = opendir(_serverConfig.getRootFolder().c_str());
	if (!dir)
		return (0);
	closedir(dir);
	if (access(_scriptPath.c_str(), X_OK) == -1)
		return (0);
	Logger::debug() << _scriptPath << " exists and is executable" << std::endl;
	return (1);
}

void CGISource::readSource() {
	if (!_doneReading) {
		Logger::debug() << "Bytes to send: " << _bytesToSend << std::endl;
		size_t bytesRead = read(_outputPipe[0], _readBuffer.data(), _serverConfig.getBufferSize());
		_body.insert(_body.end(), _readBuffer.begin(), _readBuffer.begin() + bytesRead);

		if (bytesRead == 0)
			_doneReading = true;
		_bytesToSend += bytesRead;
		_size += bytesRead;

		Logger::debug() << "Read " << bytesRead << " bytes from cgi source: " << std::endl << std::endl;
		Logger::debug() << "Bytes to send: " << _bytesToSend << std::endl;
		if (bytesRead > 0) Logger::detail() << "Data read from source: " << std::endl << std::string(_readBuffer.data(), bytesRead) << std::endl;
		if (bytesRead > 0) Logger::detail() << "Body data (size=" << _body.size() << "): " << std::endl << std::string(_body.data(), _bytesToSend) << std::endl;
		if (_doneReading) Logger::debug() << "Done reading CGI source" << std::endl;
	}
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
}
