#include "Config.hpp"
#include "Server.hpp"

int main() {
	std::string config_file = ".config";
	Config config(config_file);
	Server server(config.getServersConfigs()[0]);
}
