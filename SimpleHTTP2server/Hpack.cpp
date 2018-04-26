#include "Hpack.hpp"
#include "Huffman.hpp"

Hpack::Hpack() {}

Header Hpack::decodeHTTP2Header(char *receivedPayload, unsigned int length) {
  Header header;
  unsigned int iterator = 0;
  
  //prints out compressed header start
  std::cout << std::endl << "Header binary: ";
  for (unsigned int i = 0; i < length;i++) {std::cout << std::hex << ((int)receivedPayload[i] & 0xFF);}
  std::cout << std::endl;
  //prints out compressed header end
  
  while (iterator < length) {
    
    //Indexed Header Field Representation
    if ((receivedPayload[iterator] & 0x80) >> 7) {
      unsigned int index = receivedPayload[iterator++] & 0x7F;
      
      StaticTableLine line = StaticTable::static_table[index - 1];
      header.setHeaderline(line.header_name, line.header_value);
      
      std::cout << std::endl << "Indexed Header Field Representation, index: " << index << ", at: "  << std::dec << iterator;
    }
    
    //Literal Header Field with Incremental Indexing
    else if ((receivedPayload[iterator] & 0xC0) == 0x40) {
      unsigned int index = receivedPayload[iterator++] & 0x3F, nameLength;
      bool huffman = (receivedPayload[iterator] & 0x80) >> 7;
      nameLength = receivedPayload[iterator++] & 0x7F;
      
      char * stringData = &receivedPayload[iterator];
      iterator += nameLength;
      
      std::cout << std::endl << "Literal Header Field with Incremental Indexing, index: " << index << ", at: " << std::dec << iterator - nameLength << ", length: " << nameLength << ", huffman: " << huffman << ", binary: ";
      for (size_t j = 0; j < nameLength;j++) {
        std::cout << std::hex << ((int)stringData[j] & 0x0F);
      }
      
      std::string valueString(handeBinary(stringData, nameLength, huffman));
      header.setHeaderline(StaticTable::static_table[index - 1].header_name, valueString);
      
      std::cout << ", name: " << valueString;
    }
    
    //Literal Header Field without Indexing
    else if ((receivedPayload[iterator] & 0xF0) == 0x00) {
      unsigned int index = receivedPayload[iterator++] & 0x0F, nameLength;
      bool huffman = (receivedPayload[iterator] & 0x80) >> 7;
      nameLength = receivedPayload[iterator++] & 0x7F;
      
      char * stringData = &receivedPayload[iterator];
      iterator += nameLength;
      
      std::cout << std::endl << "Literal Header Field without Indexing: " << index << ", at: "  << std::dec << iterator  - nameLength << ", length: " << nameLength << ", huffman: " << huffman << ", binary: ";
      for (size_t j = 0; j < nameLength;j++) {
        std::cout << std::hex << ((int)stringData[j] & 0x0F);
      }
      
      std::string valueString(handeBinary(stringData, nameLength, huffman));
      header.setHeaderline(StaticTable::static_table[index - 1].header_name, valueString);
      
      std::cout << ", name: " << valueString;
    }
    
    //Literal Header Field Never Indexed
    else if ((receivedPayload[iterator] & 0xF0) == 0x10) {
      unsigned int index = receivedPayload[iterator++] & 0x0F, nameLength;
      bool huffman = (receivedPayload[iterator] & 0x80) >> 7;
      nameLength = receivedPayload[iterator++] & 0x7F;
      
      char *stringData = &receivedPayload[iterator];
      iterator += nameLength;
      
      std::cout << std::endl << "Literal Header Field Never Indexed, index: " << index << ", at: "  << std::dec << iterator - nameLength << ", length: " << nameLength << ", huffman: " << huffman << ", binary: ";
      
      for (size_t j = 0; j < nameLength;j++) {
        std::cout << std::hex << ((int)stringData[j] & 0x0F);
      }
      if (huffman) {
        //Huffman::decode(stringData, nameLength);
      }
      
      std::string valueString(handeBinary(stringData, nameLength, huffman));
      header.setHeaderline(StaticTable::static_table[index - 1].header_name, valueString);
      
      std::cout << ", name: " << valueString;
    }
    
    //Dynamic Table Size Update
    else if ((receivedPayload[iterator] & 0xE0) == 0x20) {
      unsigned int dynamicTableSizeUpdate = receivedPayload[iterator++] & 0x1F, m = 0;
      
      bool continuation = true;
      while (continuation) {
        dynamicTableSizeUpdate += (receivedPayload[iterator] & 0x7F) << m;
        m += 7;
        continuation = (receivedPayload[iterator++] & 0x80) >> 7;
      }
      
      std::cout << std::endl << "Dynamic Table Size Update, newSize: " << std::dec << dynamicTableSizeUpdate << ", at: "  << std::dec << iterator - 3;
      
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
      
    }
  }
  
  return header;
}

std::string Hpack::handeBinary(char *buffer, unsigned int length, bool huffman) {
  if (!huffman) return std::string(buffer, buffer + length);
    
  /*
  std::vector<char> outData;
  std::cout << std::string(outData.data(), outData.size()) << std::endl;
  Huffman::decode(buffer, length, outData);
  return std::string(outData.begin(), outData.end());
  */
  
  return "<undecoded>";
}
