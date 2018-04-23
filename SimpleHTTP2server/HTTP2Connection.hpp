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
#include "HTTP2Stream.hpp"

//using namespace std;

class HTTP2Connection {
private:
  
  static unsigned int connectionIdIncrement;
  
  unsigned int connectionId;
  
  int socket;
  char *buffer;
  
  std::map<unsigned int, HTTP2Stream> streams;
  
  bool ENABLE_PUSH = 0;
  unsigned int HEADER_TABLE_SIZE = 4096,
      MAX_CONCURRENT_STREAMS = 100,
      INITIAL_WINDOW_SIZE = 65535,
      MAX_HEADER_LIST_SIZE = 16384,
      MAX_FRAME_SIZE = 0;
  
  /*
  * SETTINGS_HEADER_TABLE_SIZE (0x1)
  * SETTINGS_ENABLE_PUSH (0x2)
  * SETTINGS_MAX_CONCURRENT_STREAMS (0x3)
  * SETTINGS_INITIAL_WINDOW_SIZE (0x4)
  * SETTINGS_MAX_FRAME_SIZE (0x5)
  * SETTINGS_MAX_HEADER_LIST_SIZE (0x6)
  */
  
public:
  HTTP2Connection(int socket, char *buffer);
  
  unsigned int getConnectionId();
};

#endif