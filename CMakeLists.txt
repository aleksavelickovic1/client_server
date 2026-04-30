cmake_minimum_required(VERSION 3.14)
project(ClientServerChat CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Threads REQUIRED)

find_package(Qt6 COMPONENTS Widgets QUIET)
if(Qt6_FOUND)
    set(QT_VERSION_MAJOR 6)
else()
    find_package(Qt5 COMPONENTS Widgets QUIET)
    if(Qt5_FOUND)
        set(QT_VERSION_MAJOR 5)
    endif()
endif()

set(COMMON_INCLUDES
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/common
    ${CMAKE_SOURCE_DIR}/communication
    ${CMAKE_SOURCE_DIR}/client
    ${CMAKE_SOURCE_DIR}/server
)

add_subdirectory(ui)

add_executable(server_main
    main/ServerMain.cpp
    server/Server.cpp
    server/ClientManager.cpp
    server/ClientSession.cpp
    communication/Connection.cpp
    communication/MessageBroadcaster.cpp
)
target_include_directories(server_main PRIVATE ${COMMON_INCLUDES})
target_link_libraries(server_main PRIVATE Threads::Threads)

add_executable(client_main
    main/ClientMain.cpp
    client/Client.cpp
    client/InputHandler.cpp
    client/MessageReceiver.cpp
    communication/Connection.cpp
)
target_include_directories(client_main PRIVATE ${COMMON_INCLUDES})
target_link_libraries(client_main PRIVATE Threads::Threads)

set_target_properties(client_main PROPERTIES
    BUILD_RPATH "/usr/lib/x86_64-linux-gnu"
    INSTALL_RPATH "/usr/lib/x86_64-linux-gnu"
)
if(TARGET ui)
    target_link_libraries(client_main PRIVATE ui Qt${QT_VERSION_MAJOR}::Widgets)
    target_compile_definitions(client_main PRIVATE CHAT_UI_ENABLED)
    set_target_properties(client_main PROPERTIES AUTOMOC ON)
endif()