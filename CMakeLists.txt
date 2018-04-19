cmake_minimum_required(VERSION 2.8)

project(webServer)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1y -Wall -Wextra")

SET(SOURCE
  SimpleHTTP2server/ServerLib.cpp
)
SET(HEADERS
  SimpleHTTP2server/ServerLib.hpp
)

add_library(SimpleHTTP2server ${SOURCE} ${HEADERS} )

add_executable(webServer webserver.cpp)
target_link_libraries(webServer SimpleHTTP2server)