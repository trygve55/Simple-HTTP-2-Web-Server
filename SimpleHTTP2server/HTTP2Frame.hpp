#ifndef HTTP2FRAME_HPP
#define HTTP2FRAME_HPP

#include <string>
#include <sstream>
#include <iostream>
#include <netinet/in.h>
#include <unistd.h>
#include <vector>
#include <cstdint>
#include <cstring>
#include "BufferSize.hpp"

//using namespace std;

class HTTP2Frame {
private:
  uint8_t type = 0;
  uint8_t flags = 0;
  bool reserved = false;
  int streamIdentifier = 0; 
  char payload[BUFFERSIZE] = {0};
  unsigned int length = 0;
  //char frame[1024]= {0};
  
public:
  static const struct Types {
    static const uint8_t
        DATA = 0,
        HEADERS = 1,
        PRIORITY = 2,
        RST_STREAM = 3,
        SETTINGS = 4,
        PUSH_PROMISE = 5,
        PING = 6,
        GOAWAY = 7,
        WINDOW_UPDATE = 8,
        CONTINUATION = 9;
  } FrameIDs;
  
  HTTP2Frame() {}
  HTTP2Frame(const char buffer[]);
  unsigned int const& getLength();
  void setPayload(char const payload[], unsigned int payloadSize);
  char* getPayload() {return payload;}
  unsigned int getSize();
  void setType(uint8_t type);
  void setFlags(uint8_t flags);
  void setStreamIdentifier(int newStreamIdentifier);
  uint8_t const& getType();
  uint8_t const& getFlags();
  bool getFlag(unsigned int place);
  int const& getStreamIdentifier();
  void getFrame(char *frame);
  //ssize_t sendFrame(int socket);
  void emptyPayload() {length = 0;}
  
  std::string debugFrame(char *frame);

static std::vector<HTTP2Frame> bufferToFrames(const char buffer[], const int bufferSize);
};

#endif