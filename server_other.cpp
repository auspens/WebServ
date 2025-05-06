#include <iostream>
#include <cstdlib>
#include <string>
#include <cstring>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>

#include "RequestParser.hpp"
#include "HttpResponse.hpp"

int main(int argc, char **argv) {
  // Flush after every std::cout / std::cerr
  std::cout << std::unitbuf;
  std::cerr << std::unitbuf;
  
  std::cout << "Logs from your program will appear here!\n";

  int server_fd = socket(AF_INET, SOCK_STREAM, 0);
  if (server_fd < 0) {
   std::cerr << "Failed to create server socket\n";
   return 1;
  }
  
  // Since the tester restarts your program quite often, setting SO_REUSEADDR
  // ensures that we don't run into 'Address already in use' errors
  int reuse = 1;
  if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) < 0) {
    std::cerr << "setsockopt failed\n";
    return 1;
  }
  
  struct sockaddr_in server_addr;
  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = INADDR_ANY;
  server_addr.sin_port = htons(4221);
  
  if (bind(server_fd, (struct sockaddr *) &server_addr, sizeof(server_addr)) != 0) {
    std::cerr << "Failed to bind to port 4221\n";
    return 1;
  }
  
  int connection_backlog = 5;
  if (listen(server_fd, connection_backlog) != 0) {
    std::cerr << "listen failed\n";
    return 1;
  }
  
  struct sockaddr_in client_addr;
  int client_addr_len = sizeof(client_addr);
  
  std::cout << "Waiting for a client to connect...\n";
  
  int connect_socket;
  connect_socket = accept(server_fd, (struct sockaddr *) &client_addr, (socklen_t *) &client_addr_len);
  if (connect_socket < 0)
    return 1;

  std::cout << "Client connected\n";

  char buffer[5000] = {0};
  int valread = read(connect_socket, buffer, 5000);
  RequestParser rp;
  HttpRequest req;
  if (rp.parse(buffer, valread) == RequestParser::COMPLETE)
    req = rp.getRequest();
  
  HttpResponse resp;
  resp.http_version = req.http_version;

  if (req.uri == "/") {
    resp.code = "200";
    resp.status = "OK";
  }
  else if (req.uri.substr(0,6) == "/echo/") {
    resp.body = req.uri.substr(6);
    resp.code = "200";
    resp.status = "OK";
    resp.headers["Content-Type"] = "text/plain";
    resp.headers["Content-Length"] = std::to_string(resp.body.length());
  } 
  else if (req.uri.substr(0,11) == "/user-agent") {
    resp.body = req.headers["User-Agent"];
    resp.code = "200";
    resp.status = "OK";
    resp.headers["Content-Type"] = "text/plain";
    resp.headers["Content-Length"] = std::to_string(resp.body.length());
  }
  else {
    resp.code = "404";
    resp.status = "Not Found";
  }
  
  std::string response;
  response.append(resp.http_version + " " + resp.code + " " + resp.status + "\r\n");
  for (auto it = resp.headers.begin(); it != resp.headers.end(); ++it)
    response += it->first + ": " + it->second + "\r\n";
  response += "\r\n"; // END of headers: blank line
  response += resp.body;
  
  ssize_t bytes_sent = write(connect_socket, response.c_str(), response.length());
  if (bytes_sent != (ssize_t)response.length()) {
    std::cerr << "Partial write! Only sent " << bytes_sent << " bytes out of " << response.length() << std::endl;
    // You must handle it here (loop, or error)
  }

  // Tell the peer that we finished sending
  shutdown(connect_socket, SHUT_RDWR);

  close(connect_socket);
  close(server_fd);

  //std::cout << "Response length: " << response.length() << "\nResponse: " << response << "END" << std::endl;

  return 0;
}
