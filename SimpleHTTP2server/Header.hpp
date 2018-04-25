#ifndef HEADER_HPP
#define HEADER_HPP

#include <string>
#include <map>
#include <sstream>

//using namespace std;

class Header {
private:
  std::map<std::string, std::string> headerlines;
  
public:
  
  Header();
  int setHeaderline(std::string headerline, std::string content);
  std::string getHeaderline(std::string headerline);
  std::string getHTTP();
  std::string getHTTP2_0();
  std::map<std::string, std::string> getHeaderlines();
};

#endif