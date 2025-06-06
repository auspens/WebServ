#include <iostream>
#include "Config.hpp"
#include "Server.hpp"
#include "TestConfig.hpp"

void runCGI(const std::string command, std::vector<char *> argv, std::vector<char *> envp) {
	// Execute
	std::cout << "Running execve in child process: " << command.c_str() << std::endl;
	execve(command.c_str(), argv.data(), envp.data());

	// If execve fails:
	std::cerr << "execve failed: " << strerror(errno) << std::endl;
	std::exit(1);
}

int main(int argc, char *argv[]) {
	std::string	config_file;
	Config		config;

	if (argc == 1)
		config_file = "webserv.conf";
	else if (argc == 2)
		config_file = argv[1];
	else {
		std::cout << "webserv: Too many arguments" << std::endl;
		exit(1);
	}

	try {
		config.parse(config_file);
	} catch (ConfigParseException &e) {
		std::cout << e.what() << std::endl;
		exit(1);
	}

	printFullConfig(config);

	try {
		Server server(config);
		server.listen();
	} catch (ChildProcessNeededException &e) {
		std::cout << "Calling run CGI" << std::endl;
		runCGI(e.cmd(), e.argv(), e.envp());
	}
}
