#pragma once

#include <fstream>
#include <sstream>

using namespace std;

int read_htmlfile(stringstream &bodyStream,string filePath) {
  stringstream ss;
  ifstream f;
  f.open(filePath.c_str());
  if (!f.is_open()) return -1;
  f.seekg(0, ios::end);
  int fSize = f.tellg();
  f.seekg(0, ios::beg);
  {
    string line;
    while (getline(f ,line)) {
      bodyStream << line << "\r\n";
      fSize++;
    }
  }
  f.close();
  return fSize;
};
