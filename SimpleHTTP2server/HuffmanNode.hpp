#include <stdint.h>
#include <string>
#include <sstream>

class HuffmanNode {
private:
  bool bit, newByte = false;
  char value;
  int freq;
  HuffmanNode *leftNode, *rightNode;

public:
  //HuffmanNode() {}
  
  HuffmanNode(uint8_t value, int freq, HuffmanNode* leftNode, HuffmanNode* rightNode) : value(value), freq(freq) {
    this->leftNode = leftNode;
    this->rightNode = rightNode;
  }

  HuffmanNode(int freq, HuffmanNode* leftNode, HuffmanNode* rightNode) {
    this->freq = freq;
    this->leftNode = leftNode;
    this->rightNode = rightNode;
  }

  HuffmanNode(char value, int freq, HuffmanNode* leftNode, HuffmanNode* rightNode, bool newByte) {
    this->value = value;
    this->freq = freq;
    this->leftNode = leftNode;
    this->rightNode = rightNode;
    this->newByte = newByte;
  }

  uint8_t getValue() { return value; }

  int getFreq() { return freq; }

  HuffmanNode* getLeftNode() { return leftNode; }

  HuffmanNode* getRightNode() { return rightNode; }

  bool isBit() { return bit; }

  bool isNewByte() { return newByte; }

  void setBit(bool bit) { this->bit = bit; }
  
  std::string toString() {
    std::stringstream ss;
    ss << "this: " << this << ", value: " << getValue() << ", freq:" << freq << ", left node: "<< getLeftNode() << ", right node: " << getRightNode() << "\n";
    return ss.str();
  }
};