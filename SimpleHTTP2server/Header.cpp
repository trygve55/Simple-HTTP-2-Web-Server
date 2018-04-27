#include "Header.hpp"

Header::Header() {
  
}

int Header::setHeaderline(std::string headerline, std::string content) {
  headerlines[headerline] = content;
  
  return 0;
}

std::string Header::getHeaderline(std::string headerline) {
  return headerlines[headerline];
}

std::string Header::getHTTP() {
  std::stringstream ss;
  ss << getHeaderline(":method") << " " << getHeaderline(":path") << " " << getHeaderline(":protocol") << "\r\n";
  
  for(std::map<std::string, std::string>::iterator it = headerlines.begin(); it != headerlines.end(); ++it) {
    
    
    if (it -> first.at(0) != ':') ss << it -> first << ": " << it -> second << "\r\n";;
  }
  
  ss << "\r\n\r\n";
  
  return ss.str();
}

std::string Header::getHTTP2_0() {
  std::stringstream ss;
  
  for(std::map<std::string, std::string>::iterator it = headerlines.begin(); it != headerlines.end(); ++it) {
    if (it -> first.compare("host") == 0) ss << ':';
    ss << it -> first << ": " << it -> second << "\r\n";
  }
  
  ss << "\r\n\r\n";
  
  return ss.str();
}

std::vector<char> Header::getBinary(bool huffman) {
  std::vector<char> outData;
  
  for(std::map<std::string, std::string>::iterator it = headerlines.begin(); it != headerlines.end(); ++it) {
    outData.emplace_back(0x0); //
    outData.emplace_back(it -> first.length() & 0x7F); //huffman bit and Name length
    
    //Name string
    for (auto c: it -> first) {
      outData.emplace_back(c);
    }
    
    outData.emplace_back(it -> second.length() & 0x7F); //huffman bit and Value length
    
    //Value string
    for (auto c: it -> second) {
      outData.emplace_back(c);
    }
  }
  
  return outData;
}

std::map<std::string, std::string> Header::getHeaderlines() {
  return headerlines;
}