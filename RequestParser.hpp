#pragma once

#include "HttpRequest.hpp"
#include <string>
#include <sstream>

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
        ParseResult parse(const char* data, size_t len);
        bool isDone() const;
        const HttpRequest& getRequest() const;
        void reset();
    
    private:
        ParseState state;
        HttpRequest request;
        std::string buffer;

        bool parseStartLine();
        bool parseHeaders();
        bool parseBody(); //does not matter for GET, on which we focus for now

        size_t contentLength;
    };
    