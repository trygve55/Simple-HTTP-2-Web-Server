cmake_minimum_required(VERSION 2.8)

project(webServer)

#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1y -Wall -Wextra -pthread")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1y -Wall -Wextra -pthread -Wunused-but-set-variable -Wno-unused-variable")

find_package(Threads REQUIRED)

SET(SOURCE
  SimpleHTTP2server/ServerLib.cpp
  SimpleHTTP2server/Header.cpp
  SimpleHTTP2server/HTTP2Connection.cpp
  SimpleHTTP2server/HTTP2ConnectionSettings.cpp
  SimpleHTTP2server/HTTP2Frame.cpp
  SimpleHTTP2server/HTTP2Stream.cpp
  SimpleHTTP2server/WebBinder.cpp
  SimpleHTTP2server/Hpack.cpp
  SimpleHTTP2server/Huffman.cpp
  SimpleHTTP2server/HpackStaticTable.cpp
)
SET(HEADERS
  SimpleHTTP2server/ServerLib.hpp
  SimpleHTTP2server/Header.hpp
  SimpleHTTP2server/HeaderParser.hpp
  SimpleHTTP2server/HTTP2Connection.hpp
  SimpleHTTP2server/HTTP2ConnectionSettings.hpp
  SimpleHTTP2server/HTTP2Frame.hpp
  SimpleHTTP2server/HTTP2Stream.hpp
  SimpleHTTP2server/WebBinder.hpp
  SimpleHTTP2server/Hpack.hpp
  SimpleHTTP2server/ReadFile.hpp
  SimpleHTTP2server/HpackStaticTable.hpp
  SimpleHTTP2server/Huffman.hpp
  SimpleHTTP2server/HuffmanNode.hpp
  SimpleHTTP2server/HuffmanBitString.hpp
)

add_library(SimpleHTTP2server ${SOURCE} ${HEADERS} )

add_executable(webServer webserver.cpp)
target_link_libraries(webServer SimpleHTTP2server)