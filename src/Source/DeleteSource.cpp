
#include "DeleteSource.hpp"

DeleteSource::DeleteSource(
	const ServerConfig &serverConfig,
	Location const &location,
	HttpRequest &req,
	std::string target
) throw(SourceAndRequestException) :
	Source(serverConfig, &location, req)
{
	Logger::debug() << "Creating Delete source"<<std::endl;
	_target = target;
}

DeleteSource::DeleteSource(const DeleteSource &other):Source(other){}

DeleteSource &DeleteSource::operator=(const DeleteSource &other){
	if (this != &other){
		Source::operator=(other);
	}
	return *this;
}

void DeleteSource::init() throw(SourceAndRequestException) {
	Source::init();

	_doneReading = true;

	_checkAccess(_target);
	_deleteFile();
	setHeader();
}

void DeleteSource::_deleteFile() throw(SourceAndRequestException) {
	if (access(_target.c_str(), W_OK))
		throw SourceAndRequestException("Forbidden", 403);
	if (std::remove(_target.c_str()) != 0)
		throw SourceAndRequestException("Could not delete file", 500);
}

void DeleteSource::setHeader() {
	std::string response;

	response =
		"HTTP/1.1 200 OK\r\n"
		"Content-Type: application/json\r\n"
		"Content-Length: 42\r\n"
		"\r\n"
		"{\"message\": \"File deleted successfully\"}\r\n";

	_body.assign(response.begin(), response.end());
	_bytesToSend = response.size();
}

DeleteSource::~DeleteSource() { }

void DeleteSource::readSource() { }
