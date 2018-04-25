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

std::string Header::getHTTP1_1() {
  
}
std::string Header::getHTTP2_0() {
  
}
std::map<std::string, std::string> Header::getHeaderlines() {
  return headerlines;
}