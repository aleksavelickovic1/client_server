find_package(Qt6 COMPONENTS Widgets QUIET)
if(Qt6_FOUND)
    set(QT_VERSION_MAJOR 6)
else()
    find_package(Qt5 COMPONENTS Widgets QUIET)
    if(Qt5_FOUND)
        set(QT_VERSION_MAJOR 5)
    endif()
endif()

if(Qt${QT_VERSION_MAJOR}_FOUND)
    message(STATUS "Qt found — building with UI support")

    add_library(ui
        ChatWindow.cpp
        UiClient.cpp
        UiMessageReceiver.cpp
    )

    set_target_properties(ui PROPERTIES
        AUTOMOC ON
        AUTOUIC ON
        AUTORCC ON
    )

    target_include_directories(ui
        PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
        PRIVATE
            ${CMAKE_SOURCE_DIR}
            ${CMAKE_SOURCE_DIR}/common
            ${CMAKE_SOURCE_DIR}/communication
    )

    target_link_libraries(ui
        PRIVATE
            Qt${QT_VERSION_MAJOR}::Widgets
            Threads::Threads
    )
else()
    message(STATUS "Qt not found — building CLI only")
endif()