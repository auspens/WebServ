#pragma once

#include <stdexcept>
#include <string>
#include <vector>

class ChildProcessNeededException : std::exception {
	private:
		std::string					_cmd;
		std::vector<std::string>	_envp;
		int							_inputPipe;
		int							_outputPipe;

	public:
		ChildProcessNeededException(
			std::string cmd,
			std::vector<std::string> envp,
			int	inputPipe,
			int	outputPipe
		) throw();
		~ChildProcessNeededException() throw();
		const char						*what() const throw();
		const std::string				cmd() const throw();
		const std::vector<std::string>	envp() const throw();
		int								inputPipe();
		int								outputPipe();
};
