#pragma once
#include "Source.hpp"
#include "../SystemCallsUtilities.hpp"
#include "../WebServUtils.hpp"
#include <dirent.h>

#include <sys/wait.h>

typedef void (*CleanupFunc)(void* ctx); // a ptr to void returning function, that takes a ptr that will provide context (ctx)

class CGISource : public Source {
	public:
		void setPreExecCleanup(CleanupFunc func, void* ctx);
		void readSource();
		char* getBufferToSend();
		void forkAndExec();

		CGISource(const std::string &target, const ServerConfig &serverConfig, Location const *location);
		//copy construct missing
		~CGISource();

		int getPipeReadEnd() const;
		bool getIfExists() const;
		

	private:
		bool	_pathExists;
        int _pipefd[2];
        std::string _scriptPath;
        std::string _queryString;
        std::string _pathInfo;

		bool checkIfExists();
		CGISource(); //default constructor that is never used
		CleanupFunc _cleanupFunc; //this function and its argument will be provided by server
    	void* _cleanupCtx; //will be passed as argument to cleanupFunc_
};
