#include "CGISource.hpp"

CGISource::CGISource(const std::string &target, const ServerConfig &serverConfig, Location const &location)
 : Source(target, serverConfig), _cleanupFunc(NULL), _cleanupCtx(NULL) {
    _location = location;
    _type = CGI;
    
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

    if (pipe(_pipefd) == -1)
        perror("pipe");
    forkAndExec();
}

CGISource::~CGISource(){
	std::cout << "FileSource destructor called";
	close(_pipefd[0]);
    close(_pipefd[1]);
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
        dup2(_pipefd[1], STDOUT_FILENO); // Redirect stdout to pipe
        close(_pipefd[1]);

        

        std::string path = _serverConfig.getRootFolder() + _scriptPath;

        /// Build argv
        char* argv[] = {(char*)_scriptPath.c_str(), NULL};

        // Build environment variables
        std::vector<std::string> env_strings;
        if (_pathInfo.length())
            env_strings.push_back(std::string("PATH_INFO=") + _pathInfo);
        env_strings.push_back("REQUEST_METHOD=GET");
        env_strings.push_back(std::string("QUERY_STRING=") + _queryString);
        env_strings.push_back(std::string("SCRIPT_NAME=") + _scriptPath);
        env_strings.push_back("SERVER_PROTOCOL=HTTP/1.1");

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
        close(_pipefd[1]);  // Close write end
        int status;
        waitpid(pid, &status, 0);
    }
}

void CGISource::readSource() {
    _body.resize(1024);
    size_t bytesread = read(_pipefd[0], _body.data(), 1024);
    _body[bytesread] = '\0';
    close(_pipefd[0]);
    _size = bytesread;
    _bytesToSend = bytesread;
    _body.resize(bytesread);
    std::cout << "_body is: ";
    for (std::vector<char>::iterator it = _body.begin(); it != _body.end(); ++it)
        std::cout << *it;
    std::cout << std::endl;
}


bool CGISource::checkIfExists(){
	DIR *dir = opendir(_serverConfig.getRootFolder().c_str());
	if (!dir)
		return (0);
	closedir(dir);
	if (!access(_scriptPath.c_str(), R_OK))
        return (0);
    return (1);
}


int CGISource::getPipeReadEnd() const {
    return _pipefd[0];
}

char* CGISource::readFromSource() {
    //need to form chunks
    return static_cast<char *>(_body.data() + _offset);
}