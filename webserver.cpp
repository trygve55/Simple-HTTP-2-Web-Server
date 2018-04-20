#include <iostream>
#include <string>
#define PORT 8080
#include "SimpleHTTP2server/ServerLib.hpp"

int main() {
    ServerLib server(8080);
    
    std::string defaultResponse = "HTTP/2.0 200 OK\r\n" ;
    defaultResponse.append("Content-Type: text/html\r\n");
    defaultResponse.append("\r\n");
    defaultResponse.append("<HTML><BODY>\r\n");
    defaultResponse.append("<H1> Hilsen. Du har koblet deg opp til min enkle web-tjener </h1>\r\n");
    defaultResponse.append("</BODY></HTML>\r\n");
    
    server.setDefaultResponse(defaultResponse);
    
    if (server.startServer() != 0) std::cout << "Error starting server." << std::endl;
    
}