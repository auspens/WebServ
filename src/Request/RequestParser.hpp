#pragma once

#include "HttpRequest.hpp"
#include "SourceAndRequestException.hpp"
#include "Logger.hpp"
#include "Config.hpp"
#include <cstring>
#include <string>
#include <sstream>

class RequestParser {
	public:
		enum ParseState {
			START_LINE,
			HEADERS,
			HOST_RECEIVED,
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

		ParseResult			parse(const char* data, size_t len) throw(SourceAndRequestException);
		bool				isDone() const;
		const HttpRequest 	getRequest() const;
		void 				reset();
		void 				setMaxBody(size_t size);
		void 				setMaxHeader(unsigned int size);
		bool				checkForError(const char *data, size_t len, bool errorFound);
		ParseResult			continueParsing();
		ParseState			getParseState();
		size_t				getBodySize() const;
		void				discard();

		RequestParser(const RequestParser &other);
		RequestParser &operator=(const RequestParser &other);
		~RequestParser();

    private:
		ParseState		_state;
		size_t			_contentLength;
		size_t			_chunkSize;
		size_t			_chunkPartRead;
		bool			_inChunk;
		HttpRequest		_request;
		std::string		_buffer;
		unsigned int	_maxHeader;
		size_t			_headerSize;
		size_t			_maxBody;
		size_t			_bodySize;
		bool			_discard;

		bool parseStartLine(const char *data, size_t len) throw(SourceAndRequestException);
		bool parseHeaders(const char *data, size_t len) throw(SourceAndRequestException);
		bool parseBody(const char *data, size_t len) throw(SourceAndRequestException);
		void _parseUrl();

		bool _handleChunkedInput();
		void _parseChunkSize(const std::string& hexStr);
	};









