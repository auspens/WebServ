#pragma once
#include "Source.hpp"

class DeleteSource : public Source{
	public:
		DeleteSource(const ServerConfig &serverConfig,
			Location const *location,
			HttpRequest &req);
		~DeleteSource();
		void 	readSource();
		void 	writeSource();
	private:
		void _createHTTPResponse();
};
