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
            BAD,
			EMPTY
        };
        RequestParser();
        ParseResult parse(const char* data, size_t len) throw(SourceAndRequestException);
        bool isDone() const;
        const HttpRequest getRequest() const;
        void reset();
		void setMaxBody(size_t size);
		bool checkForError(const char *data, size_t len, bool errorFound);
		ParseResult continueParsing();

    private:
        ParseState _state;
        HttpRequest _request;
        std::string _buffer;
		size_t 	_maxBody;
        bool parseStartLine(const char *data, size_t len) throw(SourceAndRequestException);
        bool parseHeaders(const char *data, size_t len) throw(SourceAndRequestException);
        bool parseBody(const char *data, size_t len) throw(SourceAndRequestException);
        void _parseUrl();
        size_t contentLength;
	};









