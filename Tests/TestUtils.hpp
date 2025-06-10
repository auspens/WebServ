#pragma once

#include <iostream>
#include <fcntl.h>
#include <errno.h>

class TestUtils {
	public:
		static void printOpenFds();

	private:
		static bool isValidFd(int fd);
};
