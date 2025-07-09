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
		void init() throw(SourceAndRequestException);

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

		bool checkIfExists();
		void buildEnvironmentVariables(std::vector<std::string> &envp);
		CGISource();
};
