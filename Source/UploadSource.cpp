
#include "UploadSource.hpp"

UploadSource::~UploadSource() {}

UploadSource::UploadSource(
	const ServerConfig &serverConfig,
	Location const *location,
	HttpRequest &req,
	std::string target
) : Source(serverConfig, location, req) {
	Logger::debug() << "Creating UploadSource" <<std::endl;
	_target = target;
}

UploadSource::UploadSource(const UploadSource &other):Source(other){
	_uploads = other._uploads;
	_isWriting = other._isWriting;
	_writeSize = other._writeSize;
	_writeFd = other._writeFd;
}

UploadSource &UploadSource::operator=(const UploadSource &other){
	if (this != &other){
		Source::operator=(other);
		_uploads = other._uploads;
		_isWriting = other._isWriting;
		_writeSize = other._writeSize;
		_writeFd = other._writeFd;
	}
	return *this;
}

void UploadSource::init() throw(SourceAndRequestException) {
	std::string header;
	std::string boundary;

	Source::init();
	DIR *dir = opendir(Config::getRootFolder(_serverConfig, _location).c_str());
	if (!dir)
		throw SourceAndRequestException("Upload folder doesn't exist", 403);
	closedir(dir);
	_isWriting = false;
	_doneWriting = false;
	_doneReading = true;
	_writeWhenComplete = true;
	_uploadOffset = 0;
	_writeSize = Config::getClientMaxBodySize(_serverConfig, _location);

	try
	{
		header = _request.headers.at("Content-Type");
	}
	catch (std::out_of_range &e)
	{
		throw SourceAndRequestException("No Content-Type header found", 400);
	}

	boundary = _findBoundary(header);
	_getUploadFiles(boundary, _request);
}

void UploadSource::_getUploadFiles(std::string boundary, HttpRequest &req)
{
	std::string line;
	std::size_t pos = 0;
	while ((pos = req.body.find(boundary, pos)) != std::string::npos)
	{
		pos += boundary.length();
		if (req.body.substr(pos, 2) == "--")
			break;
		if (req.body.substr(pos, 2) == "\r\n")
			pos += 2;
		std::size_t header_end = req.body.find("\r\n\r\n", pos);
		if (header_end == std::string::npos)
			throw SourceAndRequestException("couldn't parse the multipart form body", 400);
		std::string headers = req.body.substr(pos, header_end - pos);
		pos = header_end + 4;
		fileToUpload fileInfo;
		std::size_t cd_pos = headers.find("Content-Disposition:");
		if (cd_pos != std::string::npos)
		{
			std::size_t fn_pos = headers.find("filename=\"", cd_pos);
			if (fn_pos != std::string::npos)
			{
				fn_pos += 10;
				std::size_t fn_end = headers.find("\"", fn_pos);
				if (fn_end == std::string::npos || fn_end == fn_pos)
					throw SourceAndRequestException("Malformed filename in Content-Disposition", 400);
				fileInfo.name = _getFileName(headers.substr(fn_pos, fn_end - fn_pos));
			}
			else
			{
				std::size_t skip = req.body.find(boundary, pos);
				if (skip == std::string::npos)
					throw SourceAndRequestException("Couldn't parse upload files", 400);
				pos = skip;
				continue;
			}
		}
		else
			throw SourceAndRequestException("No Content-Disposition header for multipart form", 400);
		std::size_t next_boundary = req.body.find(boundary, pos);
		if (next_boundary == std::string::npos)
			throw SourceAndRequestException("Couldn't parse the multipart form body", 400);
		fileInfo.body = req.body.substr(pos, next_boundary - pos - 2);
		_uploads.push_back(fileInfo);
		pos = next_boundary;
	}
}

std::string UploadSource::_getFileName(std::string token)
{
	std::size_t pos;

	WebServUtils::removeFromString(token, "..");
	WebServUtils::removeFromString(token, "/");
	WebServUtils::removeFromString(token, "\\");

	pos = token.find_last_of('.');
	if (pos == std::string::npos)
		pos = token.length();

	std::ostringstream name;
	name << _target << "/" << token.substr(0, pos) << "_" << time(NULL) << token.substr(pos);

	return name.str();
}

std::string UploadSource::_findBoundary(std::string header)
{
	size_t start;
	size_t end;

	if ((start = header.find("boundary=")) != std::string::npos)
		start += 9;
	else
		throw SourceAndRequestException("No boundary in multipart form", 400);

	if (header.at(start) == '"')
	{
		start++;
		end = header.find("\"", start);
		if (end == std::string::npos)
			throw SourceAndRequestException("Error parsing boundary in multipart form", 400);
	}
	else
		end = header.find(';', start);

	return header.substr(start, end);
}

void UploadSource::writeSource() throw(SourceAndRequestException) {
	if (!_isWriting) {
		if (_uploads.empty()) {
			_doneWriting = true;
			_createHTTPResponse();
			return;
		}
		Logger::debug()<<"Writing file: " << _uploads.at(0).name <<std::endl;
		_writeFd = open(_uploads.at(0).name.c_str(), O_RDWR | O_CREAT, DEFAULT_PERMISSIONS);
		if (_writeFd < 0)
			throw SourceAndRequestException("Could not create upload file", 500);
		_isWriting = true;
		_uploadOffset = 0;
	}

	ssize_t remaining = _uploads.at(0).body.size() - _uploadOffset;
	ssize_t bytesToWrite = std::min(_writeSize, remaining);
	ssize_t bytesWritten = write(_writeFd, _uploads.at(0).body.c_str() + _uploadOffset, bytesToWrite);

	if (bytesWritten < 0)
		throw SourceAndRequestException("Could not write to upload file", 500);

	if (bytesWritten == 0) {
		_uploads.erase(_uploads.begin());
		_isWriting = false;
		close(_writeFd);
		_writeFd = -1;
		return;
	}

	_uploadOffset += bytesWritten;
}

void UploadSource::setHeader() { }

void UploadSource::_createHTTPResponse()
{
	std::string response_body = "<html><body> File uploaded successfully!</body></html>";

	std::string header = std::string(PROTOCOL) + " 200 OK\r\n";
	header += "Content-Length: " + WebServUtils::num_to_str(response_body.size()) + "\r\n";
	header += "Content-Type: text/html\r\n";
	if (_request.isKeepAlive())
		header += "Connection: Keep-Alive\r\n";
	header += "\r\n";
	header += response_body;

	_body.assign(header.begin(), header.end());
	_doneReading = true;
	_bytesToSend += header.size();
	_offset = 0;

	Logger::debug() << "UploadSource http response: " << std::string(_body.begin(), _body.end()) << std::endl;
}

void UploadSource::readSource() {}
