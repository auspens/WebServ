
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

		void		_getUploadFiles(std::string boundary, HttpRequest &req);
		void		setHeader();
		void		_createHTTPResponse();
		std::string	_parseBoundaryDelimiter(std::string header);
		std::size_t	_skipToNextBoundary(
			const std::string &body,
			const std::string &boundary,
			std::size_t pos
		);
		std::string	_extractFilename(const std::string &headers);
		std::string	_extractFileBody(
			const std::string &body,
			const std::string &boundary,
			std::size_t pos
		);
		std::string	_generateFilePath(std::string token);
};
