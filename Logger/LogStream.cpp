#include "LogStream.hpp"

LogStream::LogStream() : _logLevel(LOG_ERROR) { }
LogStream::LogStream(LogLevel level) : _logLevel(level) { }
LogStream::LogStream(const LogStream &src) : _logLevel(src._logLevel) { }
LogStream::~LogStream() {
	if (_logLevel == LOG_ERROR)
		std::cerr << oss.str();
	else
		std::cout << oss.str();
}

LogStream &LogStream::operator=(const LogStream &src) {
	if (this != &src)
		_logLevel = src._logLevel;
	return *this;
}

bool LogStream::_shouldLog(LogLevel level) {
	return level >= LOG_LEVEL;
}
