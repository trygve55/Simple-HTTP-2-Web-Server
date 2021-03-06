#include "HTTP2Frame.hpp"

HTTP2Frame::HTTP2Frame(const char buffer[]) {
  length += buffer[0] << 16;
  length += buffer[1] << 8;
  length += buffer[2];
  type = buffer[3];
  flags = buffer[4];
  reserved = buffer[5] & (1 << 7);
  streamIdentifier += (buffer[5] << 24) & 0xE;
  streamIdentifier += buffer[6] << 16;
  streamIdentifier += buffer[7] << 8;
  streamIdentifier += buffer[8];
  //std::memcpy(&payload[0], &buffer[9], length);
  for (unsigned int  i = 0; i < length;i++) payload[i] = buffer[9+i];
}

unsigned int const& HTTP2Frame::getLength() {
  return length;
}

unsigned int HTTP2Frame::getSize() {
  return 9 + length;
}

void HTTP2Frame::setPayload(char const newPayload[], unsigned int payloadSize) {
  //std::memcpy(&payload[0], &newPayload[9], length);
  for (unsigned int  i = 0; i < payloadSize;i++) payload[i] = newPayload[i];
  length = payloadSize;
  //std::cout << "size: " << length << std::endl;
}

void HTTP2Frame::setType(uint8_t type) {
  this -> type = type;
}

void HTTP2Frame::setFlags(uint8_t flags) {
  this -> flags = flags;
}

void HTTP2Frame::setStreamIdentifier(int newStreamIdentifier) {
  streamIdentifier = newStreamIdentifier;
}

uint8_t const& HTTP2Frame::getType() {
  return type;
}

uint8_t const& HTTP2Frame::getFlags() {
  return flags;
}

int const& HTTP2Frame::getStreamIdentifier() {
  return streamIdentifier;
}

void HTTP2Frame::getFrame(char *frame) {
  frame[0] = (length >> 16) & 0xFF;
  frame[1] = (length >> 8) & 0xFF;
  frame[2] = length & 0xFF;
  frame[3] = type;
  frame[4] = flags;
  frame[5] = (streamIdentifier >> 24) & 0xFF;
  frame[6] = (streamIdentifier >> 16) & 0xFF;
  frame[7] = (streamIdentifier >> 8) & 0xFF;
  frame[8] = streamIdentifier & 0xFF;
  
  std::memcpy(&frame[9], &payload[0], length);
}

std::string HTTP2Frame::debugFrame(char *frame) {
  
  std::stringstream ss;
  for(unsigned int i=0; i<length+9; ++i) {
      ss << std::hex << (unsigned int)frame[i];
    }
  return ss.str();
}

bool HTTP2Frame::getFlag(unsigned int place) {
  return ((flags & (0x01 << place)) >> place);
}

std::vector<HTTP2Frame> HTTP2Frame::bufferToFrames(const char buffer[], const int bufferSize) {
  std::vector<HTTP2Frame> frames;
  int iterator = 0;
  unsigned int frameLength;
  while (iterator < bufferSize) {
    frameLength = 9;
    frameLength += buffer[iterator] << 16;
    frameLength += buffer[iterator + 1] << 8;
    frameLength += buffer[iterator + 2];
    
    HTTP2Frame frame(&buffer[iterator]);
    frames.emplace_back(frame);
    
    iterator += frameLength;
  }
  
  return frames;
}
