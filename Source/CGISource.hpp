#pragma once

#include <ctime>
#include <dirent.h>
#include <sys/wait.h>
#include "IsChildProcessException.hpp"
#include "Logger.hpp"
#include "Source.hpp"
#include "SystemCallsUtilities.hpp"
#include "WebServUtils.hpp"

class CGISource : public Source {
	public:
		CGISource(
			const ServerConfig &serverConfig,
			Location const *location,
			HttpRequest &req,
			std::string target
		);
		//copy construct missing
		~CGISource();

		void readSource() throw(SourceAndRequestException);
		void writeSource();
		void init() throw(SourceAndRequestException);
		bool getIfExists() const;
		bool checkTimeout(int timeout) const;

		static std::map<pid_t, int> outputPipeWriteEnd;
		static std::map<pid_t, int> exitStatus;

	private:
		bool				_pathExists;
		int					_outputPipe[2];
		int					_inputPipe[2];
		std::vector<char>	_readBuffer;
		std::string			_queryString;
		std::string			_pathInfo;
		size_t				_writeOffset;
		int					_childPid;
		int					_childLastActive;

		bool _checkIfExists();
		void _buildArgv(std::vector<std::string> &argv);
		void _buildEnvp(std::vector<std::string> &envp);
		void _forkAndExec() throw(IsChildProcessException);
		bool _childProcessHealthy();
		CGISource();
};
