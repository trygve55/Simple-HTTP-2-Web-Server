#include "ServerLib.hpp"

ServerLib::ServerLib(int newPort) {
  port = newPort;
}

int ServerLib::startServer() {
  
  // Creating socket file descriptor
  if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
  {
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
  address.sin_port = htons( port );
    
  // Forcefully attaching socket to the port 8080
  if (bind(server_fd, (struct sockaddr *)&address, 
                               sizeof(address))<0)
  {
      perror("bind failed");
      return -3;
  }
  if (listen(server_fd, 3) < 0)
  {
      perror("listen");
      return -41;
  }
  
  while (1) {
      handleRequest();
  }
  
  return 0;
}

int ServerLib::handleRequest() {
    int new_socket;
    std::string res = defaultResponse;
    
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0) {
        perror("accept");
        return -1;
    }
    
    std::thread ([this, &new_socket, res]() {
        char buffer[1024] = {0};
        
        std::cout << "connected" << std::endl;
          
        read(new_socket, buffer, 1024);
        
        int headerEnd = findHeaderEnd(buffer);
        
        if (true) {
            handleHTTP2Request(new_socket, buffer);
        } else {
            std::cout << std::string(buffer) << std::endl;
            
            write(new_socket, res.c_str(), res.size());
            std::cout << "response sent"<< std::endl;
            
            close(new_socket);
        }
    }).detach();
  
    return 0;
}

int ServerLib::handleHTTP2Request(int new_socket, char buffer[1024]) {
    
    std::cout << "Upgraded to HTTP2" << std::endl;
    close(new_socket);
    
    return 0;
}

int ServerLib::setDefaultResponse(std::string response) {
    //
    defaultResponse = response;
    return 0;
}

void ServerLib::setDebug(bool debug) {
    debugFlag = debug;
}

int ServerLib::findHeaderEnd(char buffer[1024]) {
    return 0;
}

int ServerLib::findStringInCharArray(std::string string, char buffer[1024]) {
    
    return 0;
}
