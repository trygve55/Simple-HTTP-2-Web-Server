#include "Huffman.hpp"

HuffmanNode *Huffman::rootNode = Huffman::genRootNode();

HuffmanNode* Huffman::genRootNode() {
  
  Huffman::rootNode = new HuffmanNode();
  HuffmanNode *currentNode;
  std::cout << "Generated huffman tree." << std::endl;
  
  for (size_t i = 0; i < 256; i++) {
    std::cout << i << "\t" << (char)i << "\t";
    currentNode = rootNode;
    for (int j = 0; j < huffmanCodeLength[i]; j++) {
      std::cout << (huffmanCodes[i] >> (huffmanCodeLength[i] - j - 1) & 0x1);
      if (j == huffmanCodeLength[i] - 1) {
        if (huffmanCodes[i] >> (huffmanCodeLength[i] - j + 1) & 0x1) currentNode -> leftNode = new HuffmanNode(i);
        else currentNode -> rightNode = new HuffmanNode(i);
        break;
      } else if (huffmanCodes[i] >> (huffmanCodeLength[i] - j - 1) & 0x1) {
        if (currentNode -> leftNode == nullptr) currentNode -> leftNode = new HuffmanNode();
        currentNode = currentNode -> leftNode;
      } else {
        if (currentNode -> rightNode == nullptr) currentNode -> rightNode = new HuffmanNode();
        currentNode = currentNode -> rightNode;
      }
    }
  }
  
  /*
  currentNode = rootNode;
  std::cout << currentNode -> leftNode << " " << currentNode -> rightNode << " " << (int)currentNode -> getValue() << std::endl;
  currentNode = currentNode -> rightNode;
  std::cout << currentNode -> leftNode << " " << currentNode -> rightNode << " " << (int)currentNode -> getValue() << std::endl;
  currentNode = currentNode -> rightNode;
  std::cout << currentNode -> leftNode << " " << currentNode -> rightNode << " " << (int)currentNode -> getValue() << std::endl;
  currentNode = currentNode -> leftNode;
  std::cout << currentNode -> leftNode << " " << currentNode -> rightNode << " " << (int)currentNode -> getValue() << std::endl;
  currentNode = currentNode -> rightNode;
  std::cout << currentNode -> leftNode << " " << currentNode -> rightNode << " " << (int)currentNode -> getValue() << std::endl;
  currentNode = currentNode -> rightNode;
  std::cout << currentNode -> leftNode << " " << currentNode -> rightNode << " " << (int)currentNode -> getValue() << std::endl;
  */
  
  return Huffman::rootNode;
}

//Returns a list of all Huffman strings from the Huffman tree from rootNode
std::vector<HuffmanBitString> Huffman::getHuffmanStrings() {

  std::vector<HuffmanBitString> bitSetAll;
  getStrings(rootNode, bitSetAll, std::vector<bool>(), 0);

  std::cout << "test 5" << std::endl;

  for (int i = 0; i < 256; i++) {
    std::cout << std::dec << (unsigned int)bitSetAll[i].getaByte() << '\t';
    for (auto b: bitSetAll[i].getBitSet()) std::cout << b;
    std::cout << std::endl;
  }

  return bitSetAll;
}

//Recursive help method of getHuffmanStrings(HuffmanNode rootNode).
void Huffman::getStrings(HuffmanNode* rootNode, std::vector<HuffmanBitString> &bitSets, std::vector<bool> bitSet, int depth) {

   std::cout << "Depth" << depth << std::endl;

   std::vector<bool> newBitSetLeft;
   std::vector<bool> newBitSetRight;
   
   for(size_t i = 0; i < bitSet.size(); i++) {
     std::cout << bitSet[i];
   }
   std::cout << std::endl;
   
   for(int i = 0; i < depth; i++) {
     std::cout << "Depth" << depth << " test " << i << " " << bitSet.size() << std::endl;
     newBitSetLeft.emplace_back(bitSet[i]);
     newBitSetRight.emplace_back(bitSet[i]);
   }
   

   newBitSetLeft.emplace_back(true);
   newBitSetRight.emplace_back(false);
   
   std::cout << "test2 " << rootNode -> getLeftNode() -> getLeftNode() << std::endl;

   std::cout << "test2.1" << rootNode -> getLeftNode() << (unsigned long)nullptr;
   if(rootNode -> getLeftNode() -> getLeftNode() != nullptr) {
     std::cout << " Depth " << depth << " left " << rootNode -> getLeftNode() <<std::endl;
     getStrings(rootNode -> getLeftNode(), bitSets, newBitSetLeft, depth + 1);
     std::cout <<"test4.1" << std::endl;
   }
   else {
     std::cout <<" test3.1" <<std::endl;
     bitSets.emplace_back((*rootNode -> getLeftNode()).getValue(), newBitSetLeft, depth + 1, (rootNode -> getLeftNode())->isNewByte());
   }
   
   std::cout << rootNode -> getRightNode() -> getRightNode() ;
   if(rootNode -> getRightNode() -> getRightNode()  != nullptr) {
     std::cout << "Depth " << depth << " right" << std::endl;
     getStrings(rootNode -> getRightNode(), bitSets, newBitSetRight, depth + 1);
     std::cout <<"test4.2" <<std::endl;
   }
   else {
     std::cout <<"test3.2" <<std::endl;
     bitSets.emplace_back((*rootNode -> getRightNode()).getValue(), newBitSetRight, depth + 1, (rootNode -> getRightNode())->isNewByte());
   }
 }

void Huffman::decode(char *inputData, unsigned int length, std::vector<char> &outputData) {

  std::cout << "test0" << std::endl;
  std::vector<bool> bitSet;
  int j = 0;
  for (unsigned int i = 0; i < length; i++) {
    for (unsigned int j = 0; j < 8; j++) {
      bitSet.emplace_back((inputData[i] >> (7 - j)) & 0x01);
      std::cout << ((inputData[i] >> (7 - j)) & 0x01);
    }
  }
  
  std::cout << " size: " << bitSet.size() << std::endl;
  unsigned int inIteratorBit = 0;

  std::cout << "test2" << std::endl;
  
  HuffmanNode* currentNode = rootNode;
  std::cout << "test3 " << currentNode << std::endl;
  
  while(inIteratorBit < bitSet.size()) {
    std::cout << bitSet[inIteratorBit];// ((bitSet[inIteratorByte] >> (7 - inIteratorBit)) & 0x1);
    if(bitSet[inIteratorBit]) { //bitSet[inIteratorByte] >> (8 - inIteratorBit)) & 0x1) {
      if (currentNode->getLeftNode()) currentNode = currentNode->getLeftNode();
      
      if (currentNode->getLeftNode() == nullptr) {
        std::cout << std::endl << "2 char " << std::dec << (unsigned int)currentNode->getValue() << std::endl; //<< std::dec << outputData.size() << " " << (unsigned int)currentNode->getValue() << std::endl;
        outputData.emplace_back(currentNode->getValue());
        currentNode = rootNode;
      }
    }
    else {
      if (currentNode->getRightNode()) currentNode = currentNode->getRightNode();
      
      if (currentNode->getRightNode() == nullptr) {
        std::cout << std::endl << "1 char " << std::dec << (unsigned int)currentNode->getValue() << std::endl; // std::dec << outputData.size() << " " << (unsigned int)currentNode->getValue() << std::endl;
        outputData.emplace_back(currentNode->getValue());
        currentNode = rootNode;
      }
    }
    
    inIteratorBit++;
  }
  
  std::cout << "test6 "<< std::string(outputData.begin(), outputData.end());
}

const uint32_t Huffman::huffmanCodes[256] = {
	0x1ff8,
	0x7fffd8,
	0xfffffe2,
	0xfffffe3,
	0xfffffe4,
	0xfffffe5,
	0xfffffe6,
	0xfffffe7,
	0xfffffe8,
	0xffffea,
	0x3ffffffc,
	0xfffffe9,
	0xfffffea,
	0x3ffffffd,
	0xfffffeb,
	0xfffffec,
	0xfffffed,
	0xfffffee,
	0xfffffef,
	0xffffff0,
	0xffffff1,
	0xffffff2,
	0x3ffffffe,
	0xffffff3,
	0xffffff4,
	0xffffff5,
	0xffffff6,
	0xffffff7,
	0xffffff8,
	0xffffff9,
	0xffffffa,
	0xffffffb,
	0x14,
	0x3f8,
	0x3f9,
	0xffa,
	0x1ff9,
	0x15,
	0xf8,
	0x7fa,
	0x3fa,
	0x3fb,
	0xf9,
	0x7fb,
	0xfa,
	0x16,
	0x17,
	0x18,
	0x0,
	0x1,
	0x2,
	0x19,
	0x1a,
	0x1b,
	0x1c,
	0x1d,
	0x1e,
	0x1f,
	0x5c,
	0xfb,
	0x7ffc,
	0x20,
	0xffb,
	0x3fc,
	0x1ffa,
	0x21,
	0x5d,
	0x5e,
	0x5f,
	0x60,
	0x61,
	0x62,
	0x63,
	0x64,
	0x65,
	0x66,
	0x67,
	0x68,
	0x69,
	0x6a,
	0x6b,
	0x6c,
	0x6d,
	0x6e,
	0x6f,
	0x70,
	0x71,
	0x72,
	0xfc,
	0x73,
	0xfd,
	0x1ffb,
	0x7fff0,
	0x1ffc,
	0x3ffc,
	0x22,
	0x7ffd,
	0x3,
	0x23,
	0x4,
	0x24,
	0x5,
	0x25,
	0x26,
	0x27,
	0x6,
	0x74,
	0x75,
	0x28,
	0x29,
	0x2a,
	0x7,
	0x2b,
	0x76,
	0x2c,
	0x8,
	0x9,
	0x2d,
	0x77,
	0x78,
	0x79,
	0x7a,
	0x7b,
	0x7ffe,
	0x7fc,
	0x3ffd,
	0x1ffd,
	0xffffffc,
	0xfffe6,
	0x3fffd2,
	0xfffe7,
	0xfffe8,
	0x3fffd3,
	0x3fffd4,
	0x3fffd5,
	0x7fffd9,
	0x3fffd6,
	0x7fffda,
	0x7fffdb,
	0x7fffdc,
	0x7fffdd,
	0x7fffde,
	0xffffeb,
	0x7fffdf,
	0xffffec,
	0xffffed,
	0x3fffd7,
	0x7fffe0,
	0xffffee,
	0x7fffe1,
	0x7fffe2,
	0x7fffe3,
	0x7fffe4,
	0x1fffdc,
	0x3fffd8,
	0x7fffe5,
	0x3fffd9,
	0x7fffe6,
	0x7fffe7,
	0xffffef,
	0x3fffda,
	0x1fffdd,
	0xfffe9,
	0x3fffdb,
	0x3fffdc,
	0x7fffe8,
	0x7fffe9,
	0x1fffde,
	0x7fffea,
	0x3fffdd,
	0x3fffde,
	0xfffff0,
	0x1fffdf,
	0x3fffdf,
	0x7fffeb,
	0x7fffec,
	0x1fffe0,
	0x1fffe1,
	0x3fffe0,
	0x1fffe2,
	0x7fffed,
	0x3fffe1,
	0x7fffee,
	0x7fffef,
	0xfffea,
	0x3fffe2,
	0x3fffe3,
	0x3fffe4,
	0x7ffff0,
	0x3fffe5,
	0x3fffe6,
	0x7ffff1,
	0x3ffffe0,
	0x3ffffe1,
	0xfffeb,
	0x7fff1,
	0x3fffe7,
	0x7ffff2,
	0x3fffe8,
	0x1ffffec,
	0x3ffffe2,
	0x3ffffe3,
	0x3ffffe4,
	0x7ffffde,
	0x7ffffdf,
	0x3ffffe5,
	0xfffff1,
	0x1ffffed,
	0x7fff2,
	0x1fffe3,
	0x3ffffe6,
	0x7ffffe0,
	0x7ffffe1,
	0x3ffffe7,
	0x7ffffe2,
	0xfffff2,
	0x1fffe4,
	0x1fffe5,
	0x3ffffe8,
	0x3ffffe9,
	0xffffffd,
	0x7ffffe3,
	0x7ffffe4,
	0x7ffffe5,
	0xfffec,
	0xfffff3,
	0xfffed,
	0x1fffe6,
	0x3fffe9,
	0x1fffe7,
	0x1fffe8,
	0x7ffff3,
	0x3fffea,
	0x3fffeb,
	0x1ffffee,
	0x1ffffef,
	0xfffff4,
	0xfffff5,
	0x3ffffea,
	0x7ffff4,
	0x3ffffeb,
	0x7ffffe6,
	0x3ffffec,
	0x3ffffed,
	0x7ffffe7,
	0x7ffffe8,
	0x7ffffe9,
	0x7ffffea,
	0x7ffffeb,
	0xffffffe,
	0x7ffffec,
	0x7ffffed,
	0x7ffffee,
	0x7ffffef,
	0x7fffff0,
	0x3ffffee,
};
	
const uint8_t Huffman::huffmanCodeLength[256] = {
	13, 23, 28, 28, 28, 28, 28, 28, 28, 24, 30, 28, 28, 30, 28, 28,
	28, 28, 28, 28, 28, 28, 30, 28, 28, 28, 28, 28, 28, 28, 28, 28,
	6, 10, 10, 12, 13, 6, 8, 11, 10, 10, 8, 11, 8, 6, 6, 6,
	5, 5, 5, 6, 6, 6, 6, 6, 6, 6, 7, 8, 15, 6, 12, 10,
	13, 6, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
	7, 7, 7, 7, 7, 7, 7, 7, 8, 7, 8, 13, 19, 13, 14, 6,
	15, 5, 6, 5, 6, 5, 6, 6, 6, 5, 7, 7, 6, 6, 6, 5,
	6, 7, 6, 5, 5, 6, 7, 7, 7, 7, 7, 15, 11, 14, 13, 28,
	20, 22, 20, 20, 22, 22, 22, 23, 22, 23, 23, 23, 23, 23, 24, 23,
	24, 24, 22, 23, 24, 23, 23, 23, 23, 21, 22, 23, 22, 23, 23, 24,
	22, 21, 20, 22, 22, 23, 23, 21, 23, 22, 22, 24, 21, 22, 23, 23,
	21, 21, 22, 21, 23, 22, 23, 23, 20, 22, 22, 22, 23, 22, 22, 23,
	26, 26, 20, 19, 22, 23, 22, 25, 26, 26, 26, 27, 27, 26, 24, 25,
	19, 21, 26, 27, 27, 26, 27, 24, 21, 21, 26, 26, 28, 27, 27, 27,
	20, 24, 20, 21, 22, 21, 21, 23, 22, 22, 25, 25, 24, 24, 26, 23,
	26, 27, 26, 26, 27, 27, 27, 27, 27, 28, 27, 27, 27, 27, 27, 26,
};