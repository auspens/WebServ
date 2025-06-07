#include <iostream>
#include "Config.hpp"
#include "Server.hpp"
#include "TestConfig.hpp"

#include <stdexcept>

void runCGI(
	const std::string command,
	std::vector<std::string> envp_str,
	int inputPipe,
	int outputPipe
) {
	std::vector<char *> argv;
	std::vector<char*> envp;

	std::cout << "Child: In runCGI, preparing to run: " << command.c_str() << std::endl;

	argv.push_back((char *)command.c_str());
	argv.push_back(NULL);

	for (size_t i = 0; i < envp_str.size(); i++) {
		envp.push_back((char *)envp_str[i].c_str());
	}
	envp.push_back(NULL);

	std::cout << "Child: Running execve in child process: " << command.c_str() << std::endl;
	dup2(outputPipe, STDOUT_FILENO); // Redirect stdout to output pipe
	close(outputPipe);
	dup2(inputPipe, STDIN_FILENO);
	close(inputPipe);
	execve(command.c_str(), argv.data(), envp.data());

	// If execve fails:
	std::cerr << "Child: execve failed: " << strerror(errno) << std::endl;
	std::exit(1);
}

void readConfig(int argc, char *argv[], Config &config) {
	std::string	config_file;

	if (argc == 1)
		config_file = "webserv.conf";
	else if (argc == 2)
		config_file = argv[1];
	else {
		std::cout << "webserv: Too many arguments" << std::endl;
		std::exit(1);
	}

	try {
		config.parse(config_file);
	} catch (ConfigParseException &e) {
		std::cout << e.what() << std::endl;
		std::exit(1);
	}
}

int main(int argc, char *argv[]) {
	Config		config;

	readConfig(argc, argv, config);
	printFullConfig(config);

	Server *server = new Server(config);
	try {
		server->listen();
	} catch (ChildProcessNeededException &e) {
		std::cout << "Child: Calling run CGI" << std::endl;
		delete server;
		runCGI(e.cmd(), e.envp(), e.inputPipe(), e.outputPipe());
	}
	delete server;
}
