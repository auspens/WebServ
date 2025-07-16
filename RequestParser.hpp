#pragma once
#include "HttpRequest.hpp"
#include "Source/SourceAndRequestException.hpp"
#include "Logger/Logger.hpp"
#include "Config/Config.hpp"
#include <cstring>
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
		void initMaxBody(const Config &config);
		ParseState getParseState();

    private:
        ParseState _state;
		size_t 		_contentLength;
		size_t		_chunkSize;
        HttpRequest _request;
        std::string _buffer;
		size_t 		_maxBody;

        bool parseStartLine(const char *data, size_t len) throw(SourceAndRequestException);
        bool parseHeaders(const char *data, size_t len) throw(SourceAndRequestException);
        bool parseBody(const char *data, size_t len) throw(SourceAndRequestException);
        void _parseUrl();

		bool _handleChunkedInput();
		void _parseChunkSize(const std::string& hexStr);
	};









