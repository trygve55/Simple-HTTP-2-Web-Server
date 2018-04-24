#include "HTTP2Stream.hpp"

HTTP2Stream::HTTP2Stream(const int streamIdentifier) {
  this -> streamIdentifier = streamIdentifier;
}

HTTP2Stream::HTTP2Stream() {
  this -> streamIdentifier = 0;
}

void HTTP2Stream::setState(const unsigned int newState) {
  state = newState;
  
  /* 0 - idle
   * 1 - reserved (local)
   * 2 - reserved (remote)
   * 3 - open
   * 4 - half closed (remote)
   * 5 - half closed (local)
   * 6 - closed
   * */
}

unsigned int HTTP2Stream::getStage() {
  return state;
}

void HTTP2Stream::setStreamIdentifier(const int streamIdentifier) {
  this -> streamIdentifier = streamIdentifier;
}

int HTTP2Stream::getStreamIdentifier() {
  return streamIdentifier;
}