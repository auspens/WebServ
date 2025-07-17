#include <iostream>
#include "Config.hpp"
#include "Logger.hpp"
#include "Server.hpp"
#include <signal.h>
#include <sys/wait.h>

static void sigchld_handler(int sig) {
	(void)sig;
	write(Server::childProcessMonitorPipe[1], "-", 1);
}

void runCGI(
	const std::vector<std::string> argv_str,
	const std::vector<std::string> envp_str,
	int inputPipe,
	int outputPipe
) {
	std::vector<char *> argv;
	std::vector<char *> envp;

	Logger::debug() << "Child: In runCGI, preparing to run: " << argv_str[0] << std::endl;

	for (size_t i = 0; i < argv_str.size(); i++)
		argv.push_back((char *)argv_str[i].c_str());
	argv.push_back(NULL);

	for (size_t i = 0; i < envp_str.size(); i++)
		envp.push_back((char *)envp_str[i].c_str());
	envp.push_back(NULL);

	dup2(outputPipe, STDOUT_FILENO); // Redirect stdout to output pipe
	close(outputPipe);
	dup2(inputPipe, STDIN_FILENO);
	close(inputPipe);

	execve(argv[0], argv.data(), envp.data());

	// If execve fails:
	Logger::error() << "Child: execve failed: " << strerror(errno) << std::endl;
	std::exit(1);
}

int readConfig(int argc, char *argv[], Config &config) throw(ConfigParseException) {
	std::string	config_file;

	if (argc == 1)
		config_file = "webserv.conf";
	else if (argc == 2)
		config_file = argv[1];
	else {
		Logger::error() << "webserv: Too many arguments" << std::endl;
		return 1;
	}

	try {
		config.parse(config_file);
	} catch (ConfigParseException &e) {
		Logger::error() << "Error parsing config file: " << e.what() << std::endl;
		return 1;
	}
	return 0;
}

int main(int argc, char *argv[]) {
	Config		config;

	if (readConfig(argc, argv, config))
		return 1;

	signal(SIGCHLD, sigchld_handler);
	signal(SIGPIPE, SIG_IGN);

	Server *server = new Server(config);
	try {
		server->listen();
	} catch (IsChildProcessException &e) {
		Logger::debug() << "Child: Calling run CGI" << std::endl;
		delete server;
		runCGI(e.argv(), e.envp(), e.inputPipe(), e.outputPipe());
	} catch (ListeningSocket::SocketOpenFailedException &e) {
		Logger::error() << "Failed to open listening socket" << std::endl;
	}
	delete server;
}
