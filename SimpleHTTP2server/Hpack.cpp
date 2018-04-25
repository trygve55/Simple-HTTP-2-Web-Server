#include "Hpack.hpp"

Hpack::Hpack() {}

void Hpack::decodeHeader(char *receivedPayload, unsigned int length) {
  for (unsigned int i = 0; i < length;i++) {std::cout << std::hex << ((int)receivedPayload[i] & 0xFF);}
  std::cout << std::endl;
  
  //prints out compressed header
  for (unsigned int i = 0; i < length;i++) {
    
    std::cout << std::hex << ((int)receivedPayload[i] & 0xFF);
  }
  std::cout << std::endl;
  
  unsigned int iterator = 0;
  
  while (iterator < length) {
    
    //Indexed Header Field Representation
    if ((receivedPayload[iterator] & 0x80) >> 7) {
      unsigned int index = receivedPayload[iterator++] & 0x7F;
      
      //code here pls
      StaticTableLine line = StaticTable::static_table[index - 1];
      
      std::cout << std::endl << "Indexed Header Field Representation, index: " << index << ", at: "  << std::dec << iterator << std::endl;
    }
    
    //Literal Header Field with Incremental Indexing
    else if ((receivedPayload[iterator] & 0xC0) == 0x40) {
      unsigned int index = receivedPayload[iterator++] & 0x7F, nameLength;
      bool huffman = (receivedPayload[iterator] & 0x80) >> 7;
      nameLength = receivedPayload[iterator++] & 0x7F;
      
      char * stringData = &receivedPayload[iterator];
      iterator += nameLength;
      
      std::cout << std::endl << "Literal Header Field with Incremental Indexing, index: " << index << ", at: "  << std::dec << iterator << std::endl;
      for (size_t j = 0; j < nameLength;j++) {
        std::cout << std::hex << ((int)stringData[j] & 0x0F);
      }
    }
    
    //Literal Header Field without Indexing
    else if ((receivedPayload[iterator] & 0xF0) == 0x00) {
      unsigned int index = receivedPayload[iterator++] & 0x0F, nameLength;
      bool huffman = (receivedPayload[iterator] & 0x80) >> 7;
      nameLength = receivedPayload[iterator++] & 0x7F;
      
      char * stringData = &receivedPayload[iterator];
      iterator += nameLength;
      
      std::cout << std::endl << "Literal Header Field without Indexing: " << index << ", at: "  << std::dec << iterator  - nameLength << std::endl;
      for (size_t j = 0; j < nameLength;j++) {
        std::cout << std::hex << ((int)stringData[j] & 0x0F);
      }
    }
    
    //Literal Header Field Never Indexed
    else if ((receivedPayload[iterator] & 0xF0) == 0x10) {
      unsigned int index = receivedPayload[iterator++] & 0x0F, nameLength;
      bool huffman = (receivedPayload[iterator] & 0x80) >> 7;
      nameLength = receivedPayload[iterator++] & 0x7F;
      
      char *stringData = &receivedPayload[iterator];
      iterator += nameLength;
      
      std::cout << std::endl << "Literal Header Field Never Indexed, index: " << index << ", at: "  << std::dec << iterator << std::endl;
      
      for (size_t j = 0; j < nameLength;j++) {
        std::cout << std::hex << ((int)stringData[j] & 0x0F);
      }
      
    }
    
    //Dynamic Table Size Update
    else if ((receivedPayload[iterator] & 0xE0) == 0x20) {
      std::cout << std::endl << "Size header at " << iterator  << " new " ;
      
      unsigned int dynamicTableSizeUpdate = receivedPayload[iterator++] & 0x1F, m = 0;
      
      bool continuation = true;
      while (continuation) {
        std::cout << std::hex << ((int)receivedPayload[iterator] & 0xFF);
        
        dynamicTableSizeUpdate += (receivedPayload[iterator] & 0x7F) << m;
        
        m += 7;
        continuation = (receivedPayload[iterator++] & 0x80) >> 7;
      }
      
      
      std::cout << " dynamicTableSizeUpdate: " << std::dec << dynamicTableSizeUpdate << std::endl;
      
      /*
      decode I from the next N bits
      if I < 2^N - 1, return I
      else
          M = 0
          repeat
              B = next octet
              I = I + (B & 127) * 2^M
              M = M + 7
          while B & 128 == 128
          return I
       */
      
      std::cout << std::endl;
      
    }
  }
}

/*
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
