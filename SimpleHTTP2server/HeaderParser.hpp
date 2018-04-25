#pragma once

#include "Header.hpp"
#include <iostream>

Header ParseHeader(char buffer[]) {
  Header headerObject;
  
  std::string inString(buffer), headerline, content;
  std::string headerString(inString.substr(0, inString.find("\r\n\r\n")));
  
  size_t next = headerString.find(" "), iterator = 0;
  headerObject.setMethod(headerString.substr(iterator, next - iterator));
  iterator = next + 1, next = headerString.find(" ", iterator);
  headerObject.setPath(headerString.substr(iterator, next - iterator));
  iterator = next + 1, next = headerString.find("\r\n", iterator);
  headerObject.setProtocol(headerString.substr(iterator, next - iterator));
  
  //std::cout << (int)headerObject.getMethod() << " " << headerObject.getPath() << " " << (int)headerObject.getProtocol() << std::endl;
  
  //std::cout << inString << std::endl;
  
  while(iterator < headerString.length()) {
    next = headerString.find(":", iterator);
    headerline = headerString.substr(iterator + 2, next - iterator - 2);
    iterator = next;
    next = headerString.find("\r\n", iterator);
    content = headerString.substr(iterator+ 2, next - iterator - 2);
    iterator = next;
    
    headerObject.setHeaderline(headerline, content);
    
    //std::cout << "header: " << headerline << " content: " << content << std::endl;
  }
  
  return headerObject;
}

Header ParseHeader(std::string header) {
  Header headerObject;
  
  std::cout << header << std::endl;
  
  return headerObject;
}

/*
Header ParseHeader() {
    
} */