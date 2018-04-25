#ifndef HEADER_HPP
#define HEADER_HPP

#include <string>
#include <map>

//using namespace std;

class Header {
private:
  std::map<std::string, std::string> headerlines;
  char method, protocol;
  std::string path;
  
public:
  static const struct HTTPMethods {
    static const char
        OPTIONS = 0,
        GET = 1,
        HEAD = 2,
        POST = 3, 
        PUT = 4,
        DELETE = 5,
        TRACE = 6,
        CONNECT = 7;
  } Methods;
  
  static const struct Protocols {
    static const char
        HTTP0_9 = 0,
        HTTP1_0 = 1,
        HTTP1_1 = 2,
        HTTP2_0 = 3;
  } Protocols;
  
  Header();
  int setHeaderline(std::string headerline, std::string content);
  std::string getHeaderline(std::string headerline);
  void setMethod(char method);
  void setMethod(std::string method);
  char getMethod();
  void setPath(std::string path);
  std::string getPath();
  void setProtocol(std::string protocol);
  void setProtocol(char protocol);
  char getProtocol();
};

#endif