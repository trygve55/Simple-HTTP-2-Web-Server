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
#include <stdio.h>
#include <string.h>

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
  
  std::string defaultResponse = "";
  
  int handleRequest();
  int handleHTTP2Request(int new_Socket, char buffer[1024]);
  int findHeaderEnd(char buffer[1024]);
  
public:
  std::function<int(int socket, std::string *res)> thread_action;
  ServerLib(int port);
  
  int startServer();
  int bindDir(std::string webDir, std::string diskDir);
  int setDefaultResponse(std::string response);
  void setDebug(bool debug);
  
  int parse_url(int string);
};

#endif