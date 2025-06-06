#include "ChildProcessNeededException.hpp"

ChildProcessNeededException::ChildProcessNeededException(
	std::string cmd,
	std::vector<char*> argv,
	std::vector<char*> envp) throw()
		: _cmd(cmd), _argv(argv), _envp(envp) { }

ChildProcessNeededException::~ChildProcessNeededException()  throw() { }

const char *ChildProcessNeededException::what() const throw() {
	return _cmd.c_str();
}

const std::string ChildProcessNeededException::cmd() const throw() {
	return _cmd;
}

const std::vector<char*> ChildProcessNeededException::argv() const throw() {
	return _argv;
}

const std::vector<char*> ChildProcessNeededException::envp() const throw() {
	return _envp;
}
