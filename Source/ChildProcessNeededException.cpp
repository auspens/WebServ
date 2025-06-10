#include "ChildProcessNeededException.hpp"

ChildProcessNeededException::ChildProcessNeededException(
	std::string cmd,
	std::vector<std::string> envp,
	int inputPipe,
	int outputPipe
) throw()
		: _cmd(cmd), _envp(envp), _inputPipe(inputPipe), _outputPipe(outputPipe) { }

ChildProcessNeededException::~ChildProcessNeededException()  throw() { }

const char *ChildProcessNeededException::what() const throw() {
	return _cmd.c_str();
}

const std::string ChildProcessNeededException::cmd() const throw() {
	return _cmd;
}

const std::vector<std::string> ChildProcessNeededException::envp() const throw() {
	return _envp;
}

int ChildProcessNeededException::inputPipe() {
	return _inputPipe;
}

int ChildProcessNeededException::outputPipe() {
	return _outputPipe;
}
