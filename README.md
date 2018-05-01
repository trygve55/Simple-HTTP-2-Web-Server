# Simple-HTTP-2-Web-Server

This is a simple HTTP2(h2c) web server library for Linux.
This library uses only the c++ standard template library and some C functions.

Included:
  HTTP1.1 to HTTP2 upgrade.
  HTTP2 Settings sent and received by clients and server upon connection.
  Sending HTML files.
  Decoding of HTTP2 frames.
  Management of HTTP2 streams.
  Error handling of streams and connections.
  Partial header decoding.

Useful methods:
  ServerLib(int port); - Create a server object with specified port number.
  int startServer(); - Binds the socket and starts the server.
  int webBind(std::string webDir, std::string diskDir); - Binds a path on the web server to a directory on the disk.
  int webUnbind(std::string webDir); - Unbinds a path on the web server

Needed in the future:
  No support for TLS yet.
  No support for UDP communication.
  Huffman encoding is incomplete.
  Binding of a function to a web path.

Example:
  The file "webserver.cpp" is an example of how to use this libary.

Testing:
  Test the server with nghttp or similar programs. There is a shellscript (./serverTesting/get.sh) to run a
  HTTP2 get request to the server. You can test the HTTP1.1 portion of the server with curl or a web browser.

Compiling:
  $ git clone https://github.com/trygve55/Simple-HTTP-2-Web-Server
  $ cd Simple-HTTP-2-Web-Server
  $ mkdir build
  $ cd build
  $ cmake ..
  $ make
  $ ./webserver

POSIX standard library:
  <netinet/in.h>
  <unistd.h>
  <stdint.h>

Usage of C++ Standard Library:
  Containers:
    <array>
    <bitset>
    <map>
    <vector>
    
  General:
    <chrono>
    
  Strings:
    <string>
    <regex>
    
  Streams and input/output:
    <fstream>
    <iostream>
    <sstream>
    
  Language support:
    <stdexcept>
    <new>
    
  Thread support library:
    <thread>
    
Documentation:
  HTTP2 Specification:
    Hypertext Transfer Protocol Version 2 (HTTP/2) - https://http2.github.io/http2-spec/
    HPACK: Header Compression for HTTP/2 - https://http2.github.io/http2-spec/compression.html
  
  C++ specific:
    https://stackoverflow.com
    http://www.cplusplus.com
    http://en.cppreference.com