#include <stdint.h>
#include <string>
#include <sstream>
#include <iostream>

class HuffmanNode {
private:
  bool bit, newByte = false;
  char value;

public:
  
  HuffmanNode *leftNode = nullptr, *rightNode = nullptr;
  
  HuffmanNode() {}
  
  HuffmanNode(char value, HuffmanNode* leftNode, HuffmanNode* rightNode) {
    std::cout << "left: " << leftNode << " right: " << rightNode << std::endl;
    this->value = value;
    this->leftNode = leftNode;
    this->rightNode = rightNode;
  }
  
  HuffmanNode(char value) {
    std::cout << "left: " << leftNode << " right: " << rightNode << std::endl;
    this->value = value;
  }

  HuffmanNode(HuffmanNode* leftNode, HuffmanNode* rightNode) {
    std::cout << "left: " << leftNode << " right: " << rightNode << std::endl;
    this->leftNode = leftNode;
    this->rightNode = rightNode;
  }

  char getValue() { return value; }

  HuffmanNode* getLeftNode() { return leftNode; }

  HuffmanNode* getRightNode() { return rightNode; }

  bool isBit() { return bit; }

  bool isNewByte() { return newByte; }

  void setBit(bool bit) { this->bit = bit; }
  
  std::string toString() {
    std::stringstream ss;
    ss << "this: " << this << ", value: " << getValue() << ", freq:" << ", left node: "<< getLeftNode() << ", right node: " << getRightNode() << "\n";
    return ss.str();
  }
};