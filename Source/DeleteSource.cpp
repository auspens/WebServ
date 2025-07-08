#include "DeleteSource.hpp"

DeleteSource::DeleteSource(const ServerConfig &serverConfig,
			Location const *location,
			HttpRequest &req):Source(serverConfig, location, req){
	DIR *dir = opendir(_target.c_str());
	if (dir){
		closedir(dir);
		throw SourceAndRequestException ("Trying to delete a directory", 405);
	}
	if (access(_target.c_str(), F_OK) < 0)
		throw SourceAndRequestException ("File to delete doesn't exist", 404);
	int result = std::remove(_target.c_str());
	if (result != 0)
		throw SourceAndRequestException ("Failed to delete file", 405);
	_doneReading = true;
	_createHTTPResponse();
}

DeleteSource::~DeleteSource(){};

void DeleteSource::_createHTTPResponse(){
	std::string header = std::string(PROTOCOL) + " " + WebServUtils::num_to_str(_code) + " " + _statusCodes.find(_code)->second.message + "\r\n";
	std::string response_body =  "<html><body> File deleted successfully!</body></html>";

	header += "Content-Length: " + WebServUtils::num_to_str(response_body.size()) + "\r\n";
	header += "Content-Type: text/html\r\n";
	header += "\r\n";

	_body.assign(header.begin(), header.end());
	_body.insert(_body.end(), response_body.begin(), response_body.end());
	_bytesToSend = _body.size();
	_offset = 0;

	Logger::debug() << "DeleteSource http response: " << std::string(_body.begin(), _body.end()) <<std::endl;
}

void DeleteSource::readSource(){
}

void DeleteSource::writeSource(){
}
