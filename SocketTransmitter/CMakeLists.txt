cmake_minimum_required(VERSION 3.25)
project(SocketTransmitter)

set(CMAKE_CXX_STANDARD 20)

set(
    SOURCES
    src/SocketTransmitter.cpp
)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)


# SocketTransmitter.a
add_library(SocketTransmitter_static STATIC ${SOURCES})
set_target_properties(
    SocketTransmitter_static
    PROPERTIES 
    OUTPUT_NAME SocketTransmitter
)

# SocketTransmitter.so
add_library(SocketTransmitter_shared SHARED ${SOURCES})
set_target_properties(
    SocketTransmitter_shared
    PROPERTIES 
    OUTPUT_NAME SocketTransmitter
)

# SocketTransmitter
add_executable(SocketTransmitter_executable ${SOURCES})
set_target_properties(
    SocketTransmitter_executable
    PROPERTIES
    OUTPUT_NAME SocketTransmitter
)


# VS Code CMake Tools Commands
# CMake:Configure                           : cmake -S . -B build
# CMake:Delete Cache and reconfigure        : cmake --fresh -S . -B build
# CMake:Clean Rebuild                       : cmake --build . --target clean; cmake --build . --target all
# CMake:Build Target                        : cmake --build . --target <target-name>
# CMake:Build                               : cmake --build . --target all