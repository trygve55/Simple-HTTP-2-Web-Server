#ifndef SERVERLIB_H
#define SERVERLIB_H

#include <string>
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string>
#include <thread>

class ServerLib
{
private:
  int port;
  int server_fd;
  struct sockaddr_in address;
  int opt = 1;
  int addrlen = sizeof(address);
  bool debugFlag = false;
  
  std::string defaultResponse;
  
  int handleRequest();
  int handleHTTP2Request(int new_Socket, char buffer[1024]);
  int findHeaderEnd(char buffer[1024]);
  int ServerLib::findStringInCharArray(std::string string, char buffer[1024]) 
  
public:
  ServerLib(int port);
  
  int startServer();
  int bindDir(std::string webDir, std::string diskDir);
  int setDefaultResponse(std::string response);
  void setDebug(bool debug);
};

#endif