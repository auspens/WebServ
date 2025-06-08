#include "TestUtils.hpp"

bool TestUtils::isValidFd(int fd) {
	return fcntl(fd, F_GETFD) != -1 || errno != EBADF;
}

void TestUtils::printOpenFds() {
	std::cout << "Open fds:";
	for (int i = 0; i < 500; i++) {
		if (isValidFd(i))
			std::cout << " " << i;
	}
	std::cout << std::endl;
}
