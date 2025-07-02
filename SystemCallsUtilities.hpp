#pragma once

#include <sys/types.h>
#include <sys/socket.h>
#include <stdexcept>
#include <unistd.h>
#include <iostream>
#include <netdb.h>
#include <cstring>
#include <cstdlib>
#include <errno.h>
#include <sys/epoll.h>
#include <fcntl.h>
#include "Logger.hpp"


class SystemCallsUtilities{
public:
	static void check_for_error(int return_value, std::string message);

private:
	SystemCallsUtilities();
	~SystemCallsUtilities();
};
