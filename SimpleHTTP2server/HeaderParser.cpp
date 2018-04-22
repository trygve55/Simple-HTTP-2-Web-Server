#include "Header.cpp"
#include <iostream>

int findStringInCharArray(std::string string, char buffer[], size_t bufferSize);

Header ParseHeader(char buffer[]) {
  Header headerObject;
  
  std::string inString(buffer);
  std::string headerString(inString.substr(0, inString.find("\r\n\r\n")));
  
  int next = 0;
  size_t iterator = headerString.find("\r\n");
  std::string headerline, content;
  
  std::cout << inString << std::endl;
  
  while(iterator < headerString.length()) {
    next = headerString.find(":", iterator);
    headerline = headerString.substr(iterator + 2, next - iterator - 3);
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

int findStringInCharArray(std::string string, char buffer[], size_t bufferSize) {
    if (string.length() == 0) return -2;
    if (string.length() > bufferSize) return -3;
    
    for (size_t i = 0; i < bufferSize - string.length();i++) {
        for (size_t j = 0; j < string.length();j++) {
            if (string[j] != buffer[i + j]) break;
            if (j == string.length() - 1) return i;
        }
    }
    return -1;
}