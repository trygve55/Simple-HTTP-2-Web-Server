#include <iostream>
#include <string>
#include "SimpleHTTP2server/ServerLib.hpp"
#include "SimpleHTTP2server/Huffman.hpp"

int main() {
    
    //Huffman::getHuffmanStrings();
    
    
    ServerLib server(8080);
    
    server.webBind("/", "www/");
    server.webBind("/test/", "www/");
    server.webBind("/test/*/", "www/");
    server.webBind("/hei", "www/test.html");
    
    if (server.startServer() != 0) std::cout << "Error starting server." << std::endl;
    
}