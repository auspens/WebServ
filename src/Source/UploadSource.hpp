
#include <string>
#include <fcntl.h>
#include <sys/types.h>
#include <dirent.h>
#include <ctime>
#include "Source.hpp"
#include "StatusCodesStorage.hpp"

struct fileToUpload{
	std::string body;
	std::string name;
	std::string extension;
};

class UploadSource : public Source {
	public:
		UploadSource(
			const ServerConfig &serverConfig,
			Location const *location,
			HttpRequest &req,
			std::string target
		);
		~UploadSource();
		UploadSource(const UploadSource &other);
		UploadSource &operator=(const UploadSource &other);
		void	readSource();
		void	writeSource() throw(SourceAndRequestException);
		void	init() throw(SourceAndRequestException);

	private:
		std::vector <fileToUpload> _uploads;
		bool _isWriting;
		ssize_t _writeSize;
		size_t _uploadOffset;

		void _getUploadFiles(std::string boundary, HttpRequest &req);
		std::string _getFileName(std::string token);
		void setHeader();
		void _createHTTPResponse();
		std::string _findBoundary(std::string header);
};
