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

std::map<std::string, std::string> Header::getHeaderlines() {
  return headerlines;
}