#include "CGISource.hpp"

CGISource::CGISource(const std::string &target, const ServerConfig &serverConfig, Location const &location)
 : Source(target, serverConfig) {
    _location = location;
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

    checkIfExists();
    if (pipe(_pipefd) == -1)
        perror("pipe");
}

CGISource::~CGISource(){
	std::cout << "FileSource destructor called";
	close(_pipefd[0]);
    close(_pipefd[1]);
}

void CGISource::readSource() {
    pid_t pid = fork();
    if (pid < 0) {
        perror("fork");
    } else if (pid == 0) {
        //CHILD


        close(_pipefd[0]);
        // Redirect stdout to pipe
        dup2(_pipefd[1], STDOUT_FILENO);
        close(_pipefd[1]);

        //need to somehow close listening socket and client sockets in the child...
        //not the best but temporary solution:
        for (int fd = 3; fd < 1024; ++fd) { //need proper limit instead of 1024
            close(fd);
        }

        _scriptPath = _serverConfig.getRootFolder() + _scriptPath;

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

        // Execute
        //execve("/usr/bin/python3", argv, envp.data());
        execve(_scriptPath.c_str(), argv, envp.data());

        // If execve fails:

        std::cerr << "execve failed: " << strerror(errno) << std::endl;
        std::exit(1);

    } else {
        // PARENT
        close(_pipefd[1]);  // Close write end
        _body.resize(1024);
        size_t bytesread = read(_pipefd[0], _body.data(), 1024);
        _body[bytesread] = '\0';
        close(_pipefd[0]);
        _size = bytesread;
        _bytesToSend = _size;
		_body.resize(bytesread);
        // std::cout << "_body is: ";
        // for (std::vector<char>::iterator it = _body.begin(); it != _body.end(); ++it)
        //     std::cout << *it;
        // std::cout << std::endl;

        int status;
        waitpid(pid, &status, 0);

    }
}


void CGISource::checkIfExists(){
	DIR *dir = opendir(_serverConfig.getRootFolder().c_str());
	if (!dir)
		throw (Source::SourceException("No root folder"));
	closedir(dir);
	if (!access(_scriptPath.c_str(), R_OK))
        std::cout << "Error:script does not exist" << std::endl;
}
