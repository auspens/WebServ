#include "SystemCallsUtilities.hpp"

void SystemCallsUtilities::check_for_error(int return_value){
	if (return_value < 0){
		std::cout << strerror(errno) << std::endl;
		std::exit(EXIT_FAILURE);
	}
}

