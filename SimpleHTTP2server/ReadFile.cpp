#include <fstream>
#include <sstream>

using namespace std;

string read_htmlfile(string filePath) {
  stringstream ss;
  ifstream f;
  f.open(filePath.c_str());
    if (!f.is_open()) {return "HTTP/2.0 500 Internal Server Error";}// How this work?
  f.seekg(0, ios::end);
  size_t fSize = f.tellg();
  f.seekg(0, ios::beg);
  ss << "HTTP/1.1 200 OK\r\nContent-length: " << fSize << "\r\n";
  ss << "Content-Type: text/html\r\n\r\n";
  {
    string line;
    while (getline(f ,line)) {ss << line << "\r\n";}
  }
  f.close();
  return ss.str();
};
