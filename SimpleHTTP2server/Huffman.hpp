#ifndef HUFFMAN_HPP
#define HUFFMAN_HPP

#include <stdint.h>
#include <string>
#include <vector>
#include <queue>
#include <iostream>
#include <algorithm>
#include <iterator>
#include "HuffmanNode.hpp"
#include "HuffmanBitString.hpp"

//http://www.cplusplus.com/reference/queue/priority_queue/priority_queue/
class Huffman {
  
const static uint32_t huffmanCodes[];
const static uint8_t huffmanCodeLength[];
static HuffmanNode *rootNode;
  
private:
  static HuffmanNode* genRootNode();
  static void getStrings(HuffmanNode* rootNode, std::vector<HuffmanBitString> &bitSets, std::vector<bool> bitSet, int depth);
  
public:
  static std::vector<HuffmanBitString> getHuffmanStrings();

  //Inserts the decoded bytes from inputData to outputData.
  static void decode(char *inputData, unsigned int length, std::vector<char> &outputData);
};

#endif