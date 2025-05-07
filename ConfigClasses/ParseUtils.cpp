#include "ParseUtils.hpp"

bool ParseUtils::expectChar(std::ifstream &infile, char c) {
	if (infile.peek() != c)
		return false;
	infile.get();
    if (infile.fail())
        return false;
	return true;
}
