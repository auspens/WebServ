#pragma once

#include "HttpRequest.hpp"
#include <string>
#include <sstream>
#include "Source/SourceAndRequestException.hpp"
#include "Config/Config.hpp"

#include <stdlib.h>     /* atoi */

class RequestParser {
    public:
        enum ParseState {
            START_LINE,
            HEADERS,
            BODY,
            DONE,
            ERROR
        };
        enum ParseResult {
            INCOMPLETE,
            COMPLETE,
            BAD,
			GET_CONFIGS
        };
        RequestParser();
        ParseResult parse(const char* data, size_t len) throw (SourceAndRequestException);
        bool isDone() const;
        const HttpRequest getRequest() const;
		void setLocation (const Location *location);
		void setServerConfig(const ServerConfig *serverConfig);
		const ServerConfig *getServerConfig();
        void reset();

    private:
        ParseState state;
        HttpRequest request;
        std::string buffer;
        unsigned long contentLength;
		unsigned long chunkSize;
		const Location	*_location;
		const ServerConfig *_serverConfig;
		unsigned long		maxBodySize;

        bool parseStartLine();
        bool parseHeaders();
        bool parseBody();
		void _parseUrl();
		bool _handleChunkedInput();
		void _parseChunkSize(const std::string& hexStr);
		void _checkContentLength(std::map<std::string, std::string>::iterator it);
		void getMaxBodySize();
};
