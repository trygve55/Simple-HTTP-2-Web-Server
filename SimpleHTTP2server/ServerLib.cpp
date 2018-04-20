#include "ServerLib.hpp"
#include "ReadFile.cpp"

ServerLib::ServerLib(int port) : port(port) {
  // Temporary
  thread_action = [this](int socket, std::string *res) {
      char buffer[1024] = {0};
      std::cout << "Connected" << std::endl;
        
      read(socket, buffer, 1024);
      std::cout << std::string(buffer) << std::endl;
      
      string html = read_htmlfile("www/test.html");
      write(socket, html.c_str(), html.length());
      std::cout << html << std::endl;
      
      /*write(socket, res->c_str(), res->size());*/
      std::cout << "Response sent" << std::endl;
      
      close(socket);
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
        /*std::thread ([&new_socket, res]() {
            char buffer[1024] = {0};
            
            std::cout << "connected" << std::endl;
              
            read(new_socket, buffer, 1024);
            std::cout << std::string(buffer) << std::endl;
            
            write(new_socket, res.c_str(), res.size());
            std::cout << "response sent"<< std::endl;
            
            close(new_socket);
        }).detach();*/
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
