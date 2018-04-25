#include <stdint.h>
#include <string>
#include <sstream>

class HuffmanNode {
private:
  bool bit, newByte = false;
  char value;
  unsigned int freq;
  HuffmanNode *leftNode, *rightNode;

public:
  //HuffmanNode() {}
  
  HuffmanNode(char value, unsigned int freq, HuffmanNode* leftNode, HuffmanNode* rightNode) : value(value), freq(freq) {
    this->leftNode = leftNode;
    this->rightNode = rightNode;
  }

  HuffmanNode(unsigned int freq, HuffmanNode* leftNode, HuffmanNode* rightNode) {
    this->freq = freq;
    this->leftNode = leftNode;
    this->rightNode = rightNode;
  }

  HuffmanNode(char value, unsigned int freq, HuffmanNode* leftNode, HuffmanNode* rightNode, bool newByte) {
    this->value = value;
    this->freq = freq;
    this->leftNode = leftNode;
    this->rightNode = rightNode;
    this->newByte = newByte;
  }

  char getValue() { return value; }

  unsigned int getFreq() { return freq; }

  HuffmanNode* getLeftNode() { return leftNode; }

  HuffmanNode* getRightNode() { return rightNode; }

  bool isBit() { return bit; }

  bool isNewByte() { return newByte; }

  void setBit(bool bit) { this->bit = bit; }
  
  std::string toString() {
    std::stringstream ss;
    ss << "this: " << this << ", value: " << getValue() << ", freq:" << std::dec <<  freq << ", left node: "<< getLeftNode() << ", right node: " << getRightNode() << "\n";
    return ss.str();
  }
};