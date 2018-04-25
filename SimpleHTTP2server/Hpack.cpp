#include "Hpack.hpp"

Hpack::Hpack() {
  
}

void Hpack::decodeHeader(char *receivedPayload, unsigned int length) {
  
  //prints out compressed header
  for (unsigned int i = 0; i < length;i++) {
    std::cout << std::hex << ((int)receivedPayload[i] & 0xFF);
  }
  std::cout << std::endl;
}