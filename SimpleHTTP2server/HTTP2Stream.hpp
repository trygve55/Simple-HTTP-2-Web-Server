#ifndef HTTP2STREAM_HPP
#define HTTP2STREAM_HPP

#include <string>
#include <map>

//using namespace std;

class HTTP2Stream {
private:
  unsigned int state = 0;
  int streamIdentifier;
  
public:
  HTTP2Stream(const int streamIdentifier);
  HTTP2Stream();
  void setState(const unsigned int newState);
  int getStreamIdentifier();
  void setStreamIdentifier(const int streamIdentifier);
  unsigned int getStage();
};

#endif