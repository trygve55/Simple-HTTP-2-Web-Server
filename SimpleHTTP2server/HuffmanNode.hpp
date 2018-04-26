#include <stdint.h>
#include <string>
#include <sstream>
#include <iostream>

class HuffmanNode {
private:
  bool bit, newByte = false;
  char value;
  int freq;
  HuffmanNode *leftNode = nullptr, *rightNode = nullptr;

public:
  //HuffmanNode() {}
  
  HuffmanNode(char value, int freq, HuffmanNode* leftNode, HuffmanNode* rightNode) {
    std::cout << "left: " << leftNode << " right: " << rightNode << std::endl;
    this->value = value;
    this->freq = freq;
    this->leftNode = leftNode;
    this->rightNode = rightNode;
  }

  HuffmanNode(int freq, HuffmanNode* leftNode, HuffmanNode* rightNode) {
    std::cout << "left: " << leftNode << " right: " << rightNode << std::endl;
    this->freq = freq;
    this->leftNode = leftNode;
    this->rightNode = rightNode;
  }

  HuffmanNode(char value, int freq, HuffmanNode* leftNode, HuffmanNode* rightNode, bool newByte) {
    std::cout << "left: " << leftNode << " right: " << rightNode << std::endl;
    this->value = value;
    this->freq = freq;
    this->leftNode = leftNode;
    this->rightNode = rightNode;
    this->newByte = newByte;
  }

  char getValue() { return value; }

  int getFreq() {
    //std::cout << " freq  " << freq << std::endl;
    return freq;
  }

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