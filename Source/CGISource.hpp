#pragma once
#include "Source.hpp"
#include "../SystemCallsUtilities.hpp"
#include "../WebServUtils.hpp"
#include <dirent.h>

#include <sys/wait.h>

class CGISource : public Source {
	public:
        void getContent();
		void readSource();
		CGISource(const std::string &target, const ServerConfig &serverConfig, Location const &location);
		~CGISource();

	private:
        int _pipefd[2];
        std::string _scriptPath;
        std::string _queryString;
        std::string _pathInfo;
		void checkIfExists();
};