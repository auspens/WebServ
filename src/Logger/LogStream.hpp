#pragma once

#include <iostream>
#include <sstream>
#include <string>
#include "Constants.hpp"

class LogStream {
public:
	LogStream();
	LogStream(LogLevel level);
	LogStream(const LogStream &src);
	~LogStream();

	LogStream &operator=(const LogStream &src);

	template<typename T>
	LogStream &operator<<(const T& value) {
		if (_shouldLog(_logLevel)) {
			oss << value;
		}
		return *this;
	}

	LogStream &operator<<(std::ostream& (*manip)(std::ostream&)) {
		if (_shouldLog(_logLevel)) {
			oss << manip;
		}
		return *this;
	}

private:
	std::ostringstream	oss;
	LogLevel			_logLevel;

	bool _shouldLog(LogLevel level);
};
