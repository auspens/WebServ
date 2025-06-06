#pragma once

#include <stdexcept>
#include <string>
#include <vector>

class ChildProcessNeededException : std::exception {
	private:
		std::string			_cmd;
		std::vector<char*>	_argv;
		std::vector<char*>	_envp;

	public:
		ChildProcessNeededException(std::string cmd, std::vector<char*> argv, std::vector<char*> envp) throw();
		~ChildProcessNeededException() throw();
		const char					*what() const throw();
		const std::string			cmd() const throw();
		const std::vector<char*>	argv() const throw();
		const std::vector<char*>	envp() const throw();
};
