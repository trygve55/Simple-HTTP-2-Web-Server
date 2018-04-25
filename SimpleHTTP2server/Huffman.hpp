#ifndef HUFFMAN_HPP
#define HUFFMAN_HPP

#include <stdint.h>
#include <string>
#include <vector>
#include <queue>
#include <bitset>
#include <algorithm>
#include "HuffmanNode.hpp"
#include "HuffmanBitString.hpp"

//http://www.cplusplus.com/reference/queue/priority_queue/priority_queue/
class Compare {
  bool reverse;
public:
  Compare(const bool& revparam = false) {reverse=revparam;}
  bool operator() (const int& lhs, const int&rhs) const {
    if (reverse) return (lhs > rhs);
    else return (lhs < rhs);
  }
};

class Huffman {
private:
  static int byteArrayToInt(int8_t* b) {
    return (b[3] & 0xFF) |
           (b[2] & 0xFF) << 8 |
           (b[1] & 0xFF) << 16 |
           (b[0] & 0xFF) << 24;
  }

  static int8_t *intToByteArray(int a) {
    return new int8_t[4]{
        (int8_t)((a >> 24) & 0xFF),
        (int8_t)((a >> 16) & 0xFF),
        (int8_t)((a >> 8) & 0xFF),
        (int8_t)(a & 0xFF)};
  }

  //Recursive help method of getHuffmanStrings(HuffmanNode rootNode).
  static void getStrings(HuffmanNode rootNode, std::vector<HuffmanBitString> bitSets, std::bitset<16> bitSet, int depth) {

    std::bitset<16> newBitSetLeft;
    std::bitset<16> newBitSetRight;

    for(int i = 0; i < depth; i++) {
      if(bitSet[i]) {
        newBitSetLeft.set(i);
        newBitSetRight.set(i);
      }
      else {
        newBitSetLeft.reset(i);
        newBitSetRight.reset(i);
      }
    }

    newBitSetLeft.set(depth);
    newBitSetRight.reset(depth);

    if((*rootNode.getLeftNode()).getLeftNode() != nullptr) {
      getStrings((*rootNode.getLeftNode()), bitSets, newBitSetLeft, depth + 1);
    }
    else {
      bitSets.emplace_back(new HuffmanBitString((*rootNode.getLeftNode()).getValue(), newBitSetLeft, depth + 1, (*rootNode.getLeftNode()).isNewByte()));
    }

    if((*rootNode.getRightNode()).getRightNode() != nullptr) {
      getStrings(*rootNode.getRightNode(), bitSets, newBitSetRight, depth + 1);
    }
    else {
      bitSets.emplace_back(new HuffmanBitString((*rootNode.getRightNode()).getValue(), newBitSetRight, depth + 1, (*rootNode.getRightNode()).isNewByte()));
    }
  }

public:
  static int* getFreqTable(int8_t inputBytes[], int length) {
    int freq[256];

    for(int i = 0; i < length; i++)
      freq[(int)inputBytes[i] & 0xFF]++;

    return freq;
  }

  //Returns the root of a Huffman tree made from the frequency table freq.
  static HuffmanNode getHuffmanTree(int *freq, int length) {
    std::priority_queue<int, std::vector<HuffmanNode>, Compare> nodes;

    for(int i = 0; i < 256; i++) {
      if(freq[i] > 0)
        nodes.emplace(new HuffmanNode((int8_t)i, freq[i], nullptr, nullptr));
    }

    if(length == 257) {
      nodes.emplace(new HuffmanNode((int8_t)0, freq[256], nullptr, nullptr, true));
    }

    while(nodes.size() > 1) {
      HuffmanNode nodeLeft = nodes.top();
      nodes.pop();
      HuffmanNode nodeRight = nodes.top();
      nodes.pop();

      nodes.emplace(new HuffmanNode(nodeLeft.getFreq() + nodeRight.getFreq(), &nodeLeft, &nodeRight));
    }
    HuffmanNode result = nodes.top();
    nodes.pop();
    return result;
  }

  //Returns a list of all Huffman strings from the Huffman tree from rootNode
  static std::vector<HuffmanBitString> getHuffmanStrings(HuffmanNode rootNode) {
    std::vector<HuffmanBitString> bitSetAll;
    getStrings(rootNode, bitSetAll, std::bitset<16>(), 0);

    std::sort(bitSetAll.begin(), bitSetAll.end(), [] (HuffmanBitString o1, HuffmanBitString o2) {
      return o1.length() - o2.length();
    });

    return bitSetAll;
  }

  //Returns the Huffman encoded bytes from inputData.
  static int8_t* encode(int8_t inputData[], int length) {
    int maxFreq = 0, max2Freq = 1, freqBits = 0, outIterator = 0, valueOut;
    std::bitset<16> outData;

    int* freq = getFreqTable(inputData, length);

    for(int i = 0; i < length; i++)
      if(freq[i] > maxFreq)
        maxFreq = freq[i];

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
      valueOut = freq[i];

      for(int j = 0; j < freqBits; j++) {
        if(valueOut % 2 == 1)
          outData.set(outIterator);
        outIterator++;
        valueOut = valueOut >> 1;
      }
    }

    std::vector<HuffmanBitString> huffmanBitStrings = getHuffmanStrings(getHuffmanTree(freq, length));

    HuffmanBitString* currentHuffmanBitString;
    const int bit_length = 16;
    std::bitset<bit_length> bitSet;

    for(int i = 0; i < length; i++) {
      currentHuffmanBitString = nullptr;

      for(int j = 0; j < huffmanBitStrings.size(); j++) {
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
    int8_t result[bit_length];
    for (int i = 0; i < bit_length; i++) {result[i] = outData[i];}
    return result;
  }

  //Returns the decoded bytes from inputData.
  static int8_t* decode(int8_t inputData[], int length) {

    std::bitset<32> bitSet;
    int j = 0;
    for (int8_t *i = inputData; i < i + length; i++, j++) {
      bitSet[j] = *i;
    }
    int inIterator = 0, freqBits = 0, totalSize = 0;

    for(int i = 0; i < 8; i++) {
      if(bitSet[inIterator])
        freqBits += 1 << i;
      inIterator++;
    }

    int freq_length = 256;
    int freq[freq_length];

    for(int i = 0; i < freq_length; i++) {
      for(int j = 0; j < freqBits; j++) {
        if(bitSet[inIterator])
          freq[i] += 1 << j;
        inIterator++;
      }
    }

    for(int i = 0; i < freq_length; i++)
      totalSize += freq[i];

    HuffmanNode rootNode = getHuffmanTree(freq, freq_length);
    HuffmanNode* currentNode;

    int8_t outputData[totalSize];

    for(int outIterator = 0; outIterator < totalSize; outIterator++) {
      currentNode = &rootNode;
      while(currentNode->getLeftNode() != nullptr) {
        if(bitSet[inIterator]) {
          currentNode = currentNode->getLeftNode();
        }
        else {
          currentNode = currentNode->getRightNode();
        }
        inIterator++;
      }
      outputData[outIterator] = currentNode->getValue();
    }

    return outputData;
  }
/*
  static int8_t* adaptiveEncode(int8_t[] inputData) {
    int freq[] = new int[257];
    int outIterator = 0;
    BitSet outData = new BitSet(), bitSet;
    List<HuffmanBitString> huffmanBitStrings = new ArrayList<HuffmanBitString>();
    HuffmanBitString currentHuffmanBitString = null;

    for(int inputIterator = 0; inputIterator < inputData.length; inputIterator++) {

      if(freq[(int)inputData[inputIterator] & 0xFF] == 0) {
        freq[256]++;
        freq[(int)inputData[inputIterator] & 0xFF]++;
        System.out.println("added to: " + (inputData[inputIterator] & 0xFF));
        huffmanBitStrings = getHuffmanStrings(getHuffmanTree(freq));
        System.out.println(huffmanBitStrings);

        if(inputIterator != 0) {
          for(int j = 0; j < huffmanBitStrings.size(); j++) {
            if(huffmanBitStrings.get(j).isNewByte()) {
              currentHuffmanBitString = huffmanBitStrings.get(j);
              break;
            }
          }

          for(int k = 0; k < currentHuffmanBitString.length(); k++) {
            if(currentHuffmanBitString.getBitSet().get(k))
              outData.set(outIterator);
            outIterator++;
          }
        }

        int inputByte = inputData[inputIterator];
        System.out.println(inputByte);
        for(int k = 0; k < 8; k++) {
          if(inputByte % 2 != 0) {
            outData.set(outIterator);
            System.out.print("1");
          }
          else {
            System.out.print("0");
          }
          inputByte = inputByte >>> 1;
          outIterator++;
        }
        System.out.println();
      }
      else {
        freq[(int)inputData[inputIterator] & 0xFF]++;
      }

      currentHuffmanBitString = null;


      for(int j = 0; j < huffmanBitStrings.size(); j++) {
        if(inputData[inputIterator] == huffmanBitStrings.get(j).getaByte()) {
          currentHuffmanBitString = huffmanBitStrings.get(j);
          break;
        }
      }

      bitSet = currentHuffmanBitString.getBitSet();

      for(int k = 0; k < currentHuffmanBitString.length(); k++) {
        if(bitSet.get(k))
          outData.set(outIterator);
        outIterator++;
      }
    }

    return outData.toByteArray();
  }

  static int8_t* adaptiveDecode(int8_t inputData[]) {
    int freq[] = new int[257];
    int outIterator = 0, inputIterator = 0;
    BitSet inBitSet = BitSet.valueOf(inputData);
    ArrayList<Byte> outData = new ArrayList<>();
    HuffmanNode rootNode, currentNode;

    for(int i = 0; i < inputData.length * 8; i++) {
      System.out.print((inBitSet.get(i) ? "1" : "0"));
    }
    System.out.println();

    freq[256]++;

    int value = 0;
    for(int i = 0; i < 8; i++) {
      if(inBitSet.get(inputIterator)) {
        value += 1 << i;
        System.out.print("1");
      }
      else
        System.out.print("0");
      inputIterator++;
    }
    System.out.println();

    freq[value]++;
    rootNode = getHuffmanTree(freq);
    outData.add((int8_t)value);
    System.out.println("added: " + value + " " + (char)value);
    List<HuffmanBitString> huffmanBitStrings = getHuffmanStrings(getHuffmanTree(freq));
    System.out.println(huffmanBitStrings);

    for(; inputIterator < inputData.length * 8; inputIterator++) {
      currentNode = rootNode;
      while(currentNode.getLeftNode() != null) {
        if(inBitSet.get(inputIterator)) {
          currentNode = currentNode.getLeftNode();
        }
        else {
          currentNode = currentNode.getRightNode();
        }
        inputIterator++;
      }

      if(currentNode.isNewByte()) {
        value = 0;
        for(int i = 0; i < 8; i++) {
          if(inBitSet.get(inputIterator)) {
            value += 1 << i;
            System.out.print("1");
          }
          else {
            System.out.print("0");
          }

          inputIterator++;
        }
        System.out.println();

        freq[256]++;
        freq[value]++;
        rootNode = getHuffmanTree(freq);
        outData.add((int8_t)value);
        huffmanBitStrings = getHuffmanStrings(getHuffmanTree(freq));
        System.out.println(huffmanBitStrings);

        System.out.println("pos: " + inputIterator + " added: " + value + " " + (char)value);
      }
      else {

        freq[currentNode.getValue() & 0xFF]++;
        System.out.println("value: " + (currentNode.getValue() & 0xFF));
        outData.add(currentNode.getValue());
      }
    }

    int8_t out[] = new int8_t[outData.size()];
    for(int i = 0; i < out.length; i++) {
      out[i] = outData.get(i);
    }

    return out;
  }*/
};

#endif