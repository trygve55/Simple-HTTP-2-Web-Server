#ifndef SERVERLIB_HPP
#define SERVERLIB_HPP

#include <string>
#include <iostream>
#include <netinet/in.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <thread>
#include <functional>
#include <chrono>
#include <sstream>
#include "BufferSize.hpp"
#include "WebBinder.hpp"

//using namespace std;

class ServerLib {
private:
  int port = 80;
  int server_fd = 0;
  struct sockaddr_in address;
  int opt = 1;
  int addrlen = sizeof(address);
  bool debugFlag = false;
  std::vector<std::string> url_options;
  
  WebBinder webBinder;
  
  int handleRequest();
  int handleHTTP2Request(int new_Socket, char buffer[BUFFERSIZE]);
  
public:
  std::function<int(int socket)> thread_action;
  ServerLib(int port);
  
  int startServer();
  int webBind(std::string webDir, std::string diskDir);
  int webUnbind(std::string webDir);
  void setDebug(bool debug);
  
  int parse_url(int string);
};

#endif