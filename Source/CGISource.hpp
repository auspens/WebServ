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
		void readSource();
		void forkAndExec() throw(IsChildProcessException);
		void init(const ServerConfig &serverConfig, const Location *location, HttpRequest &req)
		throw(SourceAndRequestException);

		CGISource(const ServerConfig &serverConfig, const Location *location, HttpRequest &req);
		//copy construct missing
		~CGISource();

		bool getIfExists() const;
		void writeSource();

	private:
		bool				_pathExists;
		std::vector<int>	_outputPipe;
		std::vector<int>	_inputPipe;
		std::vector<char>	_readBuffer;
		std::string			_scriptPath;
		std::string			_queryString;
		std::string			_pathInfo;
		size_t				_writeOffset;
		int					_childPid;

		bool checkIfExists();
		void buildArgv(std::vector<std::string> &argv);
		void buildEnvp(std::vector<std::string> &envp);
		CGISource();
};
