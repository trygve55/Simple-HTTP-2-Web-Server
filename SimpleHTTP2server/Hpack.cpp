#include "Hpack.hpp"
#include "HpackStaticTable.hpp"

Hpack::Hpack() {}

void Hpack::decodeHeader(char *receivedPayload, unsigned int length) {
  for (unsigned int i = 0; i < length;i++) {std::cout << std::hex << ((int)receivedPayload[i] & 0xFF);}
  std::cout << std::endl;
  
  std::vector<std::string> dynamic = std::vector<std::string>(5);
  
  for (unsigned int i = 0; i < length; i+= 2) {
    short s = receivedPayload[i], idx = receivedPayload[i + 1];
    if ((s & 0xF) == 0x08 && idx <= 61 && idx > 0) {
      StaticTableLine line = StaticTable::static_table[idx - 1];
      if (line.header_name.length() == 0) {
        // Empty
      }
      else {
        // Exists
      }
    }
  }
  
  //dynamic.emplace();
}


/*int decodeBits(uint8_t* bits, unsigned int bit_amount) {
  unsigned int i = 0;
  while (i >= bit_amount * bit_amount - 1) {
    
  }
  return 0;
}*/