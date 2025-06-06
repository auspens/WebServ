#pragma once

#include <dirent.h>
#include <sys/wait.h>
#include "../SystemCallsUtilities.hpp"
#include "../WebServUtils.hpp"
#include "ChildProcessNeededException.hpp"
#include "Source.hpp"

typedef void (*CleanupFunc)(void* ctx); // a ptr to void returning function, that takes a ptr that will provide context (ctx)

struct ChildProcessArgs {

};

class CGISource : public Source {
	public:
		void setPreExecCleanup(CleanupFunc func, void* ctx);
		void readSource();
		char* getBufferToSend();
		void forkAndExec() throw(ChildProcessNeededException);

		CGISource(const ServerConfig &serverConfig, const Location *location, HttpRequest req)  throw(ChildProcessNeededException);
		//copy construct missing
		~CGISource();

		int getInputFd() const;
		int getPipeReadEnd() const;
		bool getIfExists() const;


	private:
		bool	_pathExists;
        int _pipefd[2];
		int _inputPipe[2];
        std::string _scriptPath;
        std::string _queryString;
        std::string _pathInfo;

		bool checkIfExists();
		CGISource(); //default constructor that is never used
		CleanupFunc _cleanupFunc; //this function and its argument will be provided by server
    	void* _cleanupCtx; //will be passed as argument to cleanupFunc_
};
