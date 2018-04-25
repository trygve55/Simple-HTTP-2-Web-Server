#include <stdint.h>
#include <bitset>

class HuffmanBitString {
private:
  int8_t aByte;
  std::bitset<16> bitSet;
  int bitLength;
  bool newByte = false;

public:
  //HuffmanBitString() {};
  
  HuffmanBitString(int8_t aByte, std::bitset<16> bitSet, int bitLength) : aByte(aByte), bitLength(bitLength) {
    this->bitSet = bitSet;
  }

  HuffmanBitString(int8_t aByte, std::bitset<16> bitSet, int bitLength, bool newByte): aByte(aByte), bitLength(bitLength), newByte(newByte) {
    this->bitSet = bitSet;
  }

  int8_t getaByte() {return aByte;}

  std::bitset<16> getBitSet() {return bitSet;}

  int length() {return bitLength;}

  bool isNewByte() {return newByte;}
};