cmake_minimum_required(VERSION 3.10)

project(ClientServerChat LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -g")


include_directories(client connection server main)


add_executable(server_main main/ServerMain.cpp server/Server.cpp connection/Connection.cpp)
add_executable(client_main main/ClientMain.cpp client/Client.cpp connection/Connection.cpp)