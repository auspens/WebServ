#pragma once
#include "HttpRequest.hpp"
#include "Source/SourceAndRequestException.hpp"
#include "Logger/Logger.hpp"
#include <string>
#include <sstream>
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
			URL_READY,
            BAD
        };
        RequestParser();
        ParseResult parse(const char* data, size_t len);
        bool isDone() const;
        const HttpRequest getRequest() const;
        void reset();
		void setMaxBody(size_t size);
		ParseResult continueParsing();
		
    private:
        ParseState _state;
        HttpRequest _request;
        std::string _buffer;
		size_t 	_maxBody;
        bool parseStartLine();
        bool parseHeaders();
        bool parseBody();
        void _parseUrl();
        size_t contentLength;
	};









