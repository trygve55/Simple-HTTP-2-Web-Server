#include <stdint.h>
#include <bitset>

class HuffmanBitString {
private:
  uint8_t aByte;
  std::bitset<16> bitSet;
  int bitLength;
  bool newByte = false;

public:
  //HuffmanBitString() {};
  
  HuffmanBitString(uint8_t aByte, std::bitset<16> bitSet, int bitLength) : aByte(aByte), bitLength(bitLength) {
    this->bitSet = bitSet;
  }

  HuffmanBitString(uint8_t aByte, std::bitset<16> bitSet, int bitLength, bool newByte): aByte(aByte), bitLength(bitLength), newByte(newByte) {
    this->bitSet = bitSet;
  }

  uint8_t getaByte() {return aByte;}

  std::bitset<16> getBitSet() {return bitSet;}

  int length() {return bitLength;}

  bool isNewByte() {return newByte;}
};