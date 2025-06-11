#pragma once

#include "HttpRequest.hpp"
#include <string>
#include <sstream>
#include "Source/SourceAndRequestException.hpp"

#include <stdlib.h>     /* atoi */

#define MAX_UPLOAD_SIZE 2 * 1024*1024 //2 MB

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
            BAD
        };
        RequestParser();
        ParseResult parse(const char* data, size_t len) throw (SourceAndRequestException);
        bool isDone() const;
        const HttpRequest getRequest() const;
        void reset();

    private:
        ParseState state;
        HttpRequest request;
        std::string buffer;

        bool parseStartLine();
        bool parseHeaders();
        bool parseBody();
		void _parseUrl();
		bool _handleChunkedInput();
		void _parseChunkSize(const std::string& hexStr);
		void _checkContentLength(std::map<std::string, std::string>::iterator it);

        unsigned long contentLength;
		unsigned long chunkSize;
    };
