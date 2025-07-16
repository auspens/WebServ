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
		CGISource(const CGISource &other);
		CGISource &operator=(const CGISource &other);
		~CGISource();

		void readSource() throw(SourceAndRequestException);
		void writeSource();
		void init() throw(SourceAndRequestException);
		bool getIfExists() const;
		bool checkTimeout(int timeout) const;
		void setHeader();

		static std::map<pid_t, int> outputPipeWriteEnd;
		static std::map<pid_t, int> exitStatus;

	private:
		bool				_pathExists;
		int					_outputPipe[2];
		int					_inputPipe[2];
		std::vector<char>	_readBuffer;
		std::string			_queryString;
		std::string			_pathInfo;
		std::string			_scriptName;
		size_t				_writeOffset;
		int					_childPid;
		int					_childLastActive;
		std::string 		_extension;

		bool _checkIfExists();
		void _buildArgv(std::vector<std::string> &argv);
		void _buildEnvp(std::vector<std::string> &envp);
		void _forkAndExec() throw(IsChildProcessException);
		bool _childProcessHealthy();
		CGISource();
};
