#ifndef HTTP2STREAM_HPP
#define HTTP2STREAM_HPP

#include <string>
#include <map>

//using namespace std;

class HTTP2Stream {
private:
  unsigned int state;
  unsigned int streamIdentifier;
  
public:
  HTTP2Stream(const unsigned int streamIdentifier);
  HTTP2Stream();
  void setState(const unsigned int newState);
  void getStreamIdentifier();
  unsigned int getStage();
};

#endif