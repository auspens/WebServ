#pragma once

#include "Constants.hpp"
#include "LogStream.hpp"

class Logger {
public:
	static LogStream detail();
	static LogStream debug();
	static LogStream info();
	static LogStream warning();
	static LogStream error();
};
