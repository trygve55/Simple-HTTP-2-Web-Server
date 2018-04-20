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
