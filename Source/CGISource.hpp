#pragma once

#include <dirent.h>
#include <sys/wait.h>
#include "IsChildProcessException.hpp"
#include "Logger.hpp"
#include "Source.hpp"
#include "SystemCallsUtilities.hpp"
#include "WebServUtils.hpp"

class CGISource : public Source {
	public:
		void readSource() throw(SourceAndRequestException);
		void init() throw(SourceAndRequestException);

		CGISource(const ServerConfig &serverConfig, const Location *location, HttpRequest &req);
		//copy construct missing
		~CGISource();

		bool getIfExists() const;
		void writeSource();

		static std::map<pid_t, int> outputPipeWriteEnd;
		static std::map<pid_t, int> exitStatus;

	private:
		bool				_pathExists;
		int					_outputPipe[2];
		int					_inputPipe[2];
		std::vector<char>	_readBuffer;
		std::string			_scriptPath;
		std::string			_queryString;
		std::string			_pathInfo;
		size_t				_writeOffset;
		int					_childPid;

		bool _checkIfExists();
		void _buildArgv(std::vector<std::string> &argv);
		void _buildEnvp(std::vector<std::string> &envp);
		void _forkAndExec() throw(IsChildProcessException);
		bool _childProcessHealthy();
		CGISource();
};
