cmake_minimum_required (VERSION 2.6)
project (SockSock)

add_definitions(-std=c++11)

include_directories(Headers)

add_executable (SockClient Sources/ClientMain.cpp Sources/Client.cpp)
add_executable (SockServer Sources/ServerMain.cpp Sources/Server.cpp)