#ifndef WEBBINDER_HPP
#define WEBBINDER_HPP

#include <string>
#include <vector>
#include <utility>
#include <regex>

//using namespace std;

class WebBinder {
private:
  std::vector<std::pair<std::string, std::string>> binds;
  void replaceAll(std::string& str, const std::string& from, const std::string& to);
  bool replace(std::string& str, const std::string& from, const std::string& to);
public:
  WebBinder();
  int bind(std::string webDir, std::string diskDir);
  int unbind(std::string webDir);
  std::string getPath(std::string path);
};

#endif