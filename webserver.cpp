#include <iostream>
#include <string>
#include "SimpleHTTP2server/ServerLib.hpp"

int main() {
    ServerLib server(8080);
    
    std::cout << server.webBind("/", "www/")<< std::endl;
    std::cout << server.webBind("/test/", "www/")<< std::endl;
    std::cout << server.webBind("/test/*/", "www/")<< std::endl;
    std::cout << server.webBind("/hei", "www/test.html") << std::endl;
    
    if (server.startServer() != 0) std::cout << "Error starting server." << std::endl;
    
}