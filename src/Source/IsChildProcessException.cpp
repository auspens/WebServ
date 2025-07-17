#include "IsChildProcessException.hpp"

IsChildProcessException::IsChildProcessException(
	std::vector<std::string> argv,
	std::vector<std::string> envp,
	int inputPipe,
	int outputPipe
) throw()
		: _argv(argv), _envp(envp), _inputPipe(inputPipe), _outputPipe(outputPipe) { }

IsChildProcessException::IsChildProcessException(const IsChildProcessException &src)
	: _argv(src._argv), _envp(src._envp), _inputPipe(src._inputPipe), _outputPipe(src._outputPipe) {}

IsChildProcessException& IsChildProcessException::operator=(const IsChildProcessException &src) {
	if (this != &src) {
		_argv = src._argv;
		_envp = src._envp;
		_inputPipe = src._inputPipe;
		_outputPipe = src._outputPipe;
	}
	return *this;
}


IsChildProcessException::~IsChildProcessException()  throw() { }

const char *IsChildProcessException::what() const throw() {
	return _argv[0].c_str();
}

const std::vector<std::string> IsChildProcessException::argv() const throw() {
	return _argv;
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
