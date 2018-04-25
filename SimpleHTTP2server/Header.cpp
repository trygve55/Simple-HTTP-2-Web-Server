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

void Header::setMethod(char method) {
  this -> method = method;
}

void Header::setMethod(std::string method) {
  if (method.compare("OPTIONS") == 0) this -> method = Methods.OPTIONS;
  else if (method.compare("GET") == 0) this -> method = Methods.GET;
  else if (method.compare("HEAD") == 0) this -> method = Methods.HEAD;
  else if (method.compare("POST") == 0) this -> method = Methods.POST;
  else if (method.compare("PUT") == 0) this -> method = Methods.PUT;
  else if (method.compare("DELETE") == 0) this -> method = Methods.DELETE;
  else if (method.compare("TRACE") == 0) this -> method = Methods.TRACE;
  else if (method.compare("CONNECT") == 0) this -> method = Methods.CONNECT;
  
}

char Header::getMethod() {
  return method;
}

void Header::setPath(std::string path) {
  this -> path = path;
}

std::string Header::getPath() {
  return path;
}

void Header::setProtocol(char protocol) {
  this -> protocol = protocol;
}

void Header::setProtocol(std::string protocol) {
  if (protocol.compare("HTTP/0.9") == 0) this -> protocol = Protocols.HTTP0_9;
  if (protocol.compare("HTTP/1.0") == 0) this -> protocol = Protocols.HTTP1_0;
  if (protocol.compare("HTTP/1.1") == 0) this -> protocol = Protocols.HTTP1_1;
  if (protocol.compare("HTTP/2.0") == 0) this -> protocol = Protocols.HTTP2_0;
}

char Header::getProtocol() {
  return protocol;
}