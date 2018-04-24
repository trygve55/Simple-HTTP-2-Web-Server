#include "ServerLib.hpp"
#include "ReadFile.hpp"
#include "HeaderParser.hpp"
#include "HTTP2Frame.hpp"
#include "HTTP2Stream.hpp"
#include "HTTP2Connection.hpp"

ServerLib::ServerLib(int port) : port(port) {
  // Temporary
  thread_action = [this](int socket, std::string *res) {
      char buffer[1024] = {0};
      std::cout << "Client Connected" << std::endl;
        
      read(socket, buffer, 1024);
      
      Header header = ParseHeader(buffer);
      
      std::cout << header.getHeaderline("upgrade") << std::endl;
      
      if (header.getHeaderline("upgrade").compare("h2c") == 0) {
          //Upgrade connection to HTTP2(h2c)
          HTTP2Connection connection(socket, buffer);
      } else {
          //Respond as HTTP 1.1
          stringstream responseStream, bodyStream;
          int fSize = read_htmlfile(bodyStream, "www/test.html");
          
          if (fSize >= 0) {
              responseStream << "HTTP/1.1 200 OK\r\nContent-length: " << fSize << "\r\n";
              responseStream << "Content-Type: text/html\r\n\r\n";
          } else {
              responseStream << "HTTP/1.1 500 Internal Server Error";
          }
          
          responseStream << bodyStream.rdbuf();
          std::string response = responseStream.str();
          write(socket, response.c_str(), response.length());
          std::cout << "Response sent" << std::endl;
          
          close(socket);
      }
      return 0;
  };
}

int ServerLib::startServer() {
  
  // Creating socket file descriptor
  if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
      perror("socket failed");
      return -1;
  }
    
  // Forcefully attaching socket to the port 8080
  if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
  {
      perror("setsockopt");
      return -2;
  }
  address.sin_family = AF_INET;
  address.sin_addr.s_addr = INADDR_ANY;
  address.sin_port = htons(port);
    
  // Forcefully attaching socket to the port 8080
  if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
      perror("bind failed");
      return -3;
  }
  if (listen(server_fd, 3) < 0) {
      perror("listen");
      return -41;
  }
  
  std::cout << "Server online" << std::endl;
  while (1) {
      handleRequest();
  }
  
  return 0;
}

int ServerLib::handleRequest() {
    int new_socket;
    
    if (thread_action) {
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
            perror("accept");
            return -1;
        }

        std::thread (thread_action, new_socket, &defaultResponse).detach();// Execute the function upon connection.
      }
    return 0;
}

int ServerLib::setDefaultResponse(std::string response) {
    defaultResponse = response;
    return 0;
}

void ServerLib::setDebug(bool debug) {
    debugFlag = debug;
}
