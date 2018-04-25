#ifndef HPACK_HPP
#define HPACK_HPP

#include <string>
#include <vector>
#include <iostream>
#include <string>
#include "HpackStaticTable.hpp"
#include "Header.hpp"
class Hpack {
private:

  
public:
  
  
  Hpack();
  
  Header decodeHTTP2Header(char *buffer, unsigned int length);
  
  std::string handeBinary(char *buffer, unsigned int length, bool huffman);
};

#endif