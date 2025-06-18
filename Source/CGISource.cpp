#include "CGISource.hpp"

CGISource::CGISource(const ServerConfig &serverConfig, Location const *location, HttpRequest req)  throw(ChildProcessNeededException)
 : Source(serverConfig, location, req) {
	Logger::debug() << "Creating CGI Source" << std::endl;

	_location = location;
	_type = CGI;

	_scriptPath = req.path;

	size_t qmark = req.uri.find('?');
	if (qmark != std::string::npos) {
		_queryString = req.uri.substr(qmark + 1);
	} else {
		_queryString = "";
	}

	size_t script_end = _scriptPath.find(".py") + 3; // include ".py" //
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

void CGISource::forkAndExec()  throw(ChildProcessNeededException) {
	Logger::debug() << "in forkAndExec()" << std::endl;
	std::vector<std::string> envp;
	pid_t pid;

	_inputPipe.resize(2);
	_outputPipe.resize(2);
	pipe(_inputPipe.data());
	pipe(_outputPipe.data());

	Logger::debug() << "_inputPipe = {" << _inputPipe[0] << ", " << _inputPipe[1] << "}, _outputPipe = {" << _outputPipe[0] << ", " << _outputPipe[1] << "}" << std::endl;

	pid = fork();
	if (pid < 0) {
		perror("fork");
	} else if (pid == 0) { //CHILD
		Logger::debug() << "Child: in child!" << std::endl;

		// Build environment variables
		if (_pathInfo.length())
			envp.push_back(std::string("PATH_INFO=") + _pathInfo);
		envp.push_back(std::string("REQUEST_METHOD=") + _request.method);
		envp.push_back(std::string("QUERY_STRING=") + _queryString);
		envp.push_back(std::string("SCRIPT_NAME=") + _scriptPath);
		envp.push_back("SERVER_PROTOCOL=HTTP/1.1");

		envp.push_back(std::string("CONTENT_LENGTH=") + _request.headers["Content-Length"]);
		envp.push_back(std::string("CONTENT_TYPE=") + _request.headers["Content-Type"]);

		close(_inputPipe[1]);
		close(_outputPipe[0]);

		Logger::debug() << "Child: Throwing ChildProcessNeededException" << std::endl;
		throw ChildProcessNeededException(_scriptPath, envp, _inputPipe[0], _outputPipe[1]);
	} else { // PARENT
		Logger::debug() << "Closing child pipe ends in parent" << std::endl;
		close(_inputPipe[0]);
		close(_outputPipe[1]);
	}
}

void CGISource::readSource() {
	_body.resize(_serverConfig.getBufferSize()); // This resize seems wrong - that's because it is, fixed now.
	size_t bytesRead = read(_outputPipe[0], _body.data(), _serverConfig.getBufferSize());

	Logger::debug() << "Read " << bytesRead << " bytes from cgi source: " << std::endl << std::endl;

	if (bytesRead == 0)
		_doneReading = true;
	_size = bytesRead;
	_bytesToSend = bytesRead;
	_body.resize(bytesRead);
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

int CGISource::getPipeReadEnd() const {
	return _outputPipe[0];
}

int CGISource::getInputFd() const {
	return _inputPipe[1];
}

char* CGISource::getBufferToSend() {
	//need to form chunks
	return static_cast<char *>(_body.data() + _offset);
}
