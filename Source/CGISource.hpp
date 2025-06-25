#pragma once

#include <dirent.h>
#include <sys/wait.h>
#include "ChildProcessNeededException.hpp"
#include "Logger.hpp"
#include "Source.hpp"
#include "SystemCallsUtilities.hpp"
#include "WebServUtils.hpp"

class CGISource : public Source {
	public:
		void readSource();
		void forkAndExec() throw(ChildProcessNeededException);

		CGISource(const ServerConfig &serverConfig, const Location *location, HttpRequest req)
			throw(ChildProcessNeededException);
		//copy construct missing
		~CGISource();

		bool getIfExists() const;
		void writeToSource();

	private:
		bool				_pathExists;
		std::vector<int>	_outputPipe;
		std::vector<int>	_inputPipe;
		std::string			_scriptPath;
		std::string			_queryString;
		std::string			_pathInfo;
		std::string			_uri;

		bool checkIfExists();
		void buildEnvironmentVariables(std::vector<std::string> &envp);
		CGISource();
};
