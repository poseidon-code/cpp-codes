cmake_minimum_required(VERSION 3.25)
project(Runnable)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)


# Socket library
set(LIBSOCKET_INCLUDE_DIRECTORIES external/Socket/include)
set(LIBSOCKET_SOURCES external/Socket/src/Socket.cpp)


# Runnable (actual executable built with library sources)
add_executable(Runnable_executable
    main.cpp
    ${LIBSOCKET_SOURCES}
)
target_include_directories(Runnable_executable PRIVATE ${LIBSOCKET_INCLUDE_DIRECTORIES})
set_target_properties(
    Runnable_executable
    PROPERTIES
    OUTPUT_NAME ${PROJECT_NAME}
)
