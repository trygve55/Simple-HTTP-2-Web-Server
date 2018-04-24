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
#include "HTTP2ConnectionSettings.hpp"
#include "HTTP2Stream.hpp"
#include "HTTP2Frame.hpp"
#include "BufferSize.hpp"

//using namespace std;

class HTTP2Connection {
private:
  
  static unsigned int connectionIdIncrement;
  
  unsigned int connectionId, concurrentStreams;
  int socket;
  char *reciveBuffer, *sendBuffer;
  
  std::map<unsigned int, HTTP2Stream> streams;
  
  HTTP2ConnectionSettings settings;
  
  void connectionError(int socket, unsigned int lastOKID);
  ssize_t sendFrame(HTTP2Frame frame);
  void setStreamState(int streamIdentifier, unsigned int state);
  HTTP2Stream getStream(int streamIdentifier);
public:
  HTTP2Connection(int socket, char *buffer);
  
  unsigned int getConnectionId();
  
};

#endif