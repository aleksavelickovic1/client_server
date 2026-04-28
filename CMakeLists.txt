cmake_minimum_required(VERSION 3.14)
project(ClientServerChat CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Threads REQUIRED)

set(COMMON_INCLUDES
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/common
    ${CMAKE_SOURCE_DIR}/communication
    ${CMAKE_SOURCE_DIR}/client
    ${CMAKE_SOURCE_DIR}/server
)

add_executable(server_main
    ${CMAKE_SOURCE_DIR}/main/ServerMain.cpp
    ${CMAKE_SOURCE_DIR}/server/Server.cpp
    ${CMAKE_SOURCE_DIR}/server/ClientManager.cpp
    ${CMAKE_SOURCE_DIR}/server/ClientSession.cpp
    ${CMAKE_SOURCE_DIR}/communication/Connection.cpp
    ${CMAKE_SOURCE_DIR}/communication/MessageBroadcaster.cpp
)
target_include_directories(server_main PRIVATE ${COMMON_INCLUDES})
target_link_libraries(server_main PRIVATE Threads::Threads)

add_executable(client_main
    ${CMAKE_SOURCE_DIR}/main/ClientMain.cpp
    ${CMAKE_SOURCE_DIR}/client/Client.cpp
    ${CMAKE_SOURCE_DIR}/client/InputHandler.cpp
    ${CMAKE_SOURCE_DIR}/client/MessageReceiver.cpp
    ${CMAKE_SOURCE_DIR}/communication/Connection.cpp
)
target_include_directories(client_main PRIVATE ${COMMON_INCLUDES})
target_link_libraries(client_main PRIVATE Threads::Threads)