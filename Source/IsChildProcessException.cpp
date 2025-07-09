#include "IsChildProcessException.hpp"

IsChildProcessException::IsChildProcessException(
	std::string cmd,
	std::vector<std::string> envp,
	int inputPipe,
	int outputPipe
) throw()
		: _cmd(cmd), _envp(envp), _inputPipe(inputPipe), _outputPipe(outputPipe) { }

IsChildProcessException::~IsChildProcessException()  throw() { }

const char *IsChildProcessException::what() const throw() {
	return _cmd.c_str();
}

const std::string IsChildProcessException::cmd() const throw() {
	return _cmd;
}

const std::vector<std::string> IsChildProcessException::envp() const throw() {
	return _envp;
}

int IsChildProcessException::inputPipe() {
	return _inputPipe;
}

int IsChildProcessException::outputPipe() {
	return _outputPipe;
}
