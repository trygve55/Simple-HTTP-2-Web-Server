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
  static const struct Types {
    static const unsigned int
        NO_ERROR = 0x0,
        PROTOCOL_ERROR = 0x1,
        INTERNAL_ERROR = 0x2,
        FLOW_CONTROL_ERROR = 0x3,
        SETTINGS_TIMEOUT = 0x4,
        STREAM_CLOSED = 0x5,
        FRAME_SIZE_ERROR = 0x6,
        REFUSED_STREAM = 0x7,
        CANCEL = 0x8,
        COMPRESSION_ERROR = 0x9,
        CONNECT_ERROR = 0xa,
        ENHANCE_YOUR_CALM = 0xb,
        INADEQUATE_SECURITY = 0xc,
        HTTP_1_1_REQUIRED = 0xd;
  } ErrorCodes;
  
  unsigned int connectionId, concurrentStreams;
  int socket, lastOkStreamIdentifier;
  char *reciveBuffer, *sendBuffer;
  
  std::map<unsigned int, HTTP2Stream> streams;
  
  HTTP2ConnectionSettings settings;
  
  void streamError(int streamIdentifier, unsigned int errorCode);
  void connectionError(unsigned int lastOKID, unsigned int errorCode);
  void connectionError(unsigned int lastOKID, unsigned int errorCode, char *additionalDebugData, unsigned int additionalDebugDataLength);
  ssize_t sendFrame(HTTP2Frame frame);
  void setStreamState(int streamIdentifier, unsigned int state);
  void setStreamWeight(int streamIdentifier, char weight);
  HTTP2Stream getStream(int streamIdentifier);
  
  void proccessHeaderFrame(HTTP2Frame frame);
  
public:
  HTTP2Connection(int socket, char *buffer);
  
  unsigned int getConnectionId();
  
};

#endif