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
/*
  //Returns the Huffman encoded bytes from inputData.
  static uint8_t* encode(uint8_t inputData[], int length) {
    int maxFreq = 0, max2Freq = 1, freqBits = 0, outIterator = 0, valueOut;
    std::bitset<16> outData;

    for(int i = 0; i < length; i++)
      if(freqTable[i] > maxFreq)
        maxFreq = freqTable[i];

    while(maxFreq > max2Freq) {
      freqBits++;
      max2Freq = max2Freq << 1;
    }

    int freqBitsWrite = freqBits;
    for(int i = 0; i < 8; i++) {
      if(freqBitsWrite % 2 != 0) {
        outData.set(outIterator);
      }
      outIterator++;
      freqBitsWrite = freqBitsWrite >> 1;
    }

    for(int i = 0; i < length; i++) {
      valueOut = freqTable[i];

      for(int j = 0; j < freqBits; j++) {
        if(valueOut % 2 == 1)
          outData.set(outIterator);
        outIterator++;
        valueOut = valueOut >> 1;
      }
    }

    std::vector<HuffmanBitString> huffmanBitStrings = getHuffmanStrings(getHuffmanTree());

    HuffmanBitString* currentHuffmanBitString;
    const int bit_length = 16;
    std::bitset<bit_length> bitSet;

    for(int i = 0; i < length; i++) {
      currentHuffmanBitString = nullptr;

      for(unsigned int j = 0; j < huffmanBitStrings.size(); j++) {
        if(inputData[i] == huffmanBitStrings[j].getaByte()) {
          currentHuffmanBitString = &huffmanBitStrings[j];
          break;
        }
      }

      bitSet = currentHuffmanBitString->getBitSet();

      for(int j = 0; j < currentHuffmanBitString->length(); j++) {
        if(bitSet[j])
          outData.set(outIterator);
        outIterator++;
      }
    }
    uint8_t result[bit_length];
    for (int i = 0; i < bit_length; i++) {result[i] = outData[i];}
    return result;
  }
  
  */

  //Returns the decoded bytes from inputData.
  static void decode(char *inputData, unsigned int length, std::vector<char> &outputData);
};

#endif