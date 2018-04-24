#pragma once

#include <fstream>
#include <sstream>

using namespace std;

size_t read_htmlfile(stringstream &bodyStream,string filePath) {
  stringstream ss;
  ifstream f;
  f.open(filePath.c_str());
  if (!f.is_open()) return -1;
  f.seekg(0, ios::end);
  size_t fSize = f.tellg();
  f.seekg(0, ios::beg);
  {
    string line;
    while (getline(f ,line)) {bodyStream << line << "\r\n";}
  }
  f.close();
  return fSize;
};
