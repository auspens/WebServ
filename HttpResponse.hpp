#include <string>
#include <map>

struct HttpResponse {
    std::string http_version;
    std::string code;
    std::string status;
    std::map<std::string, std::string> headers;
    std::string body;
};