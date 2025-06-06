#include <iostream>
#include "Config.hpp"
#include "Server.hpp"
#include "TestConfig.hpp"

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

	// Server * server = new Server(config);
	// std::string command = server->start();
	// delete server;

	// if (command)
	// 	runChildProcess(command);
}
