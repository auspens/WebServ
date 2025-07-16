
#pragma once

#include <cstdio>
#include "Logger.hpp"
#include "Source.hpp"

class DeleteSource : public Source
{
	public:
		DeleteSource(
			const ServerConfig &serverConfig,
			Location const &location,
			HttpRequest &req,
			std::string target
		) throw(SourceAndRequestException);
		~DeleteSource();
		DeleteSource(const DeleteSource &other);
		DeleteSource &operator=(const DeleteSource &other);

		void init() throw(SourceAndRequestException);
		void readSource();
		void setHeader();

	private:
		void _deleteFile() throw(SourceAndRequestException);
};



