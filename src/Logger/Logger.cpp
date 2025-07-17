#include "Logger.hpp"

LogStream Logger::detail() {
	return LogStream(LOG_DETAIL);
}

LogStream Logger::debug() {
	return LogStream(LOG_DEBUG);
}

LogStream Logger::info() {
	return LogStream(LOG_INFO);
}

LogStream Logger::warning() {
	return LogStream(LOG_WARNING);
}

LogStream Logger::error() {
	return LogStream(LOG_ERROR);
}
