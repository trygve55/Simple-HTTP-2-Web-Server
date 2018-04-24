#ifndef HTTP2CONNECTION_HPP
#define HTTP2CONNECTION_HPP

#include <string>
#include <iostream>
#include <netinet/in.h>
#include <unistd.h>
#include <vector>
#include <chrono>
#include <thread>
#include <map>
#include "Settings.hpp"
#include "HTTP2Stream.hpp"

//using namespace std;

class HTTP2Connection {
private:
  
  static unsigned int connectionIdIncrement;
  
  unsigned int connectionId;
  
  int socket;
  char *buffer;
  
  std::map<unsigned int, HTTP2Stream> streams;
  
  Settings settings;
  
public:
  HTTP2Connection(int socket, char *buffer);
  
  unsigned int getConnectionId();
};

#endif