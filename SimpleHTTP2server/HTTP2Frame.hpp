#ifndef HTTP2FRAME_HPP
#define HTTP2FRAME_HPP

#include <string>
#include <stdio.h>
#include <string.h>
#include <sstream>
#include <iostream>
#include <netinet/in.h>
#include <vector>

//using namespace std;

class HTTP2Frame {
private:
  uint8_t type = 0;
  uint8_t flags = 0;
  bool reserved = false;
  unsigned int streamIdentifier = 0; 
  char payload[1024] = {0};
  unsigned int length = 0;
  //char frame[1024]= {0};
  
public:
  HTTP2Frame();
  HTTP2Frame(const char buffer[]);
  unsigned int const& getLength();
  void setPayload(char payload[], unsigned int payloadSize);
  unsigned int getSize();
  void setType(uint8_t type);
  void setFlags(uint8_t flags);
  void setStreamIdentifier(unsigned int streamIdentifier);
  uint8_t const& getType();
  uint8_t const& getFlags();
  unsigned int const& getStreamIdentifier();
  void getFrame(char *frame);
  
  std::string debugFrame(char *frame);
  
  static std::vector<HTTP2Frame> bufferToFrames(const char buffer[], const int bufferSize);
};

#endif