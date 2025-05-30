#include "CGISource.hpp"

CGISource::CGISource(const ServerConfig &serverConfig, Location const *location, HttpRequest req)
 : Source(serverConfig, location, req), _cleanupFunc(NULL), _cleanupCtx(NULL) {
    _location = location;
    _type = CGI;

    std::string target = req.path;
    size_t qmark = target.find('?');

    if (qmark != std::string::npos) {
        _scriptPath = target.substr(0, qmark);
        _queryString = target.substr(qmark + 1);
    } else {
        _scriptPath = target;
        _queryString = "";
    }

    size_t script_end = _scriptPath.find(".py") + 3; // include ".py" //
    if (script_end != std::string::npos) {
        _pathInfo = _scriptPath.substr(script_end);
        _scriptPath = _scriptPath.substr(0, script_end);
    }

    _scriptPath = _serverConfig.getRootFolder() + _scriptPath;

    _pathExists = checkIfExists();
    if (!_pathExists)
        return ;

    pipe(_inputPipe);

    if (pipe(_pipefd) != -1)
        forkAndExec();
}

CGISource::~CGISource(){
	std::cout << "CGISource destructor called" << std::endl;
    if (_pathExists)
        close(_pipefd[0]);
}

void CGISource::setPreExecCleanup(CleanupFunc func, void* ctx) {
    _cleanupFunc = func;
    _cleanupCtx = ctx;
}

void CGISource::forkAndExec() {
    std::cout << "in forkAndExec()" << std::endl;
    pid_t pid = fork();
    if (pid < 0) {
        perror("fork");
    } else if (pid == 0) {
        //CHILD
        std::cout << "in child!" << std::endl;

        close(_pipefd[0]);
        dup2(_pipefd[1], STDOUT_FILENO); // Redirect stdout to output pipe
        close(_pipefd[1]);

        // char buf[10] = {0};
        // read(_inputPipe[0], buf, 9);
        // std::cerr << "read buf from pipe: " << buf << std::endl;

        close(_inputPipe[1]);
        dup2(_inputPipe[0], STDIN_FILENO);
        close(_inputPipe[0]);

        std::string path = _scriptPath;

        /// Build argv
        char* argv[] = {(char*)_scriptPath.c_str(), NULL};

        // Build environment variables
        std::vector<std::string> env_strings;
        if (_pathInfo.length())
            env_strings.push_back(std::string("PATH_INFO=") + _pathInfo);
        env_strings.push_back(std::string("REQUEST_METHOD=") + _request.method);
        env_strings.push_back(std::string("QUERY_STRING=") + _queryString);
        env_strings.push_back(std::string("SCRIPT_NAME=") + _scriptPath);
        env_strings.push_back("SERVER_PROTOCOL=HTTP/1.1");

        env_strings.push_back(std::string("CONTENT_LENGTH=") + _request.headers["Content-Length"]);
        env_strings.push_back(std::string("CONTENT_TYPE=") + _request.headers["Content-Type"]);

        std::vector<char*> envp;
        for (size_t i = 0; i < env_strings.size(); ++i)
            envp.push_back((char*)env_strings[i].c_str());
        envp.push_back(NULL);

        if (_cleanupFunc)
            _cleanupFunc(_cleanupCtx); // cleans everything including obj we are in....

        // Execute
        //execve("/usr/bin/python3", argv, envp.data());
        execve(path.c_str(), argv, envp.data());

        // If execve fails:

        std::cerr << "execve failed: " << strerror(errno) << std::endl;
        std::exit(1);

    } else {
        // PARENT
        if (close(_inputPipe[0]) != -1)
            std::cout << "Closed read end of input pipe" << std::endl;
        if (close(_pipefd[1]) != -1)
            std::cout << "Closed write end of output pipe" << std::endl;
        //int status;
        //waitpid(pid, &status, 0); //???what was i thinking..
    }
}

void CGISource::readSource() {
    _body.resize(1024);
    size_t bytesread = read(_pipefd[0], _body.data(), 1024);
    _body[bytesread] = '\0';
    if (bytesread == 0)
        _doneReading = true;
    _size = bytesread;
    _bytesToSend = bytesread;
    _body.resize(bytesread);
    std::cout << "_body is: ";
    for (std::vector<char>::iterator it = _body.begin(); it != _body.end(); ++it)
        std::cout << *it;
    std::cout << std::endl;
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
    std::cout << _scriptPath << " exists and is executable" << std::endl;
    return (1);
}

int CGISource::getPipeReadEnd() const {
    return _pipefd[0];
}

int CGISource::getInputFd() const {
    return _inputPipe[1];
}

char* CGISource::getBufferToSend() {
    //need to form chunks
    return static_cast<char *>(_body.data() + _offset);
}
