#include <stdint.h>

class HuffmanNode {
private:
  bool bit, newByte = false;
  int8_t value;
  int freq;
  HuffmanNode *leftNode, *rightNode;

public:
  //HuffmanNode() {}
  
  HuffmanNode(int8_t value, int freq, HuffmanNode* leftNode, HuffmanNode* rightNode) : value(value), freq(freq) {
    this->leftNode = leftNode;
    this->rightNode = rightNode;
  }

  HuffmanNode(int freq, HuffmanNode* leftNode, HuffmanNode* rightNode) {
    this->freq = freq;
    this->leftNode = leftNode;
    this->rightNode = rightNode;
  }

  HuffmanNode(int8_t value, int freq, HuffmanNode* leftNode, HuffmanNode* rightNode, bool newByte) {
    this->value = value;
    this->freq = freq;
    this->leftNode = leftNode;
    this->rightNode = rightNode;
    this->newByte = newByte;
  }

  int8_t getValue() { return value; }

  int getFreq() { return freq; }

  HuffmanNode* getLeftNode() { return leftNode; }

  HuffmanNode* getRightNode() { return rightNode; }

  bool isBit() { return bit; }

  bool isNewByte() { return newByte; }

  void setBit(bool bit) { this->bit = bit; }
};