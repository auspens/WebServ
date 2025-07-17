#pragma once

#include <stdexcept>
#include <string>
#include <vector>

class IsChildProcessException : std::exception {
	private:
		std::vector<std::string>	_argv;
		std::vector<std::string>	_envp;
		int							_inputPipe;
		int							_outputPipe;

	public:
		IsChildProcessException(
			std::vector<std::string> argv,
			std::vector<std::string> envp,
			int	inputPipe,
			int	outputPipe
		) throw();

		IsChildProcessException(const IsChildProcessException &src);
		IsChildProcessException& operator=(const IsChildProcessException &src);
		~IsChildProcessException() throw();

		const char						*what() const throw();
		const std::vector<std::string>	argv() const throw();
		const std::vector<std::string>	envp() const throw();
		int								inputPipe();
		int								outputPipe();
};
