#ifndef HTTP2STREAM_HPP
#define HTTP2STREAM_HPP

#include <string>
#include <map>

//using namespace std;

class HTTP2Stream {
private:
  unsigned int state = 0;
  int streamIdentifier;
  char weight = 0x0f;
  
public:
  static const struct Types {
    static const unsigned int
        idle = 0,
        reserved_local = 1,
        reserved_remote = 2,
        open = 3, 
        half_closed_remote = 4,
        half_closed_local = 5,
        closed = 6;
  } States;
  
  HTTP2Stream(const int streamIdentifier);
  HTTP2Stream();
  void setState(const unsigned int newState);
  void setWeight(const char);
  int getStreamIdentifier();
  void setStreamIdentifier(const int streamIdentifier);
  unsigned int getStage();
  char getWeight();
};

#endif