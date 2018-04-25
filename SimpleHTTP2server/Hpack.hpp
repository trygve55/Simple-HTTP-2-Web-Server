#ifndef HPACK_HPP
#define HPACK_HPP

#include <string>
#include <vector>
#include <iostream>
#include <string>
#include "HpackStaticTable.hpp"
class Hpack {
private:

  
public:
  
  
  Hpack();
  
  void decodeHeader(char *buffer, unsigned int length);
  
};

#endif