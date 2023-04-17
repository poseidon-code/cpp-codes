cmake_minimum_required(VERSION 3.25)
project(SocketTransmitter)

set(CMAKE_CXX_STANDARD 17)

set(
    SOURCES
    SocketTransmitter.cpp
    src/Socket.cpp
)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)


# SocketTransmitter.a
add_library(SocketTransmitter_static STATIC ${SOURCES})
target_include_directories(SocketTransmitter_static PUBLIC include)
set_target_properties(
    SocketTransmitter_static
    PROPERTIES 
    OUTPUT_NAME SocketTransmitter
)

# SocketTransmitter.so
add_library(SocketTransmitter_shared SHARED ${SOURCES})
target_include_directories(SocketTransmitter_shared PUBLIC include)
set_target_properties(
    SocketTransmitter_shared
    PROPERTIES 
    OUTPUT_NAME SocketTransmitter
)

# SocketTransmitter
add_executable(SocketTransmitter_executable ${SOURCES})
target_include_directories(SocketTransmitter_executable PUBLIC include)
set_target_properties(
    SocketTransmitter_executable
    PROPERTIES
    OUTPUT_NAME SocketTransmitter
)


# VS Code CMake Tools Commands
# CMake:Configure                                           : cmake -S . -B build
# CMake:Delete Cache and reconfigure                        : cmake --fresh -S . -B build
# CMake:Clean Rebuild                                       : cmake --build . --target clean; cmake --build . --target all
# CMake:Build Target                                        : cmake --build . --target <target-name>
# CMake:Build                               (F7)            : cmake --build . --target all
# CMake:Run without Debugging               (Shift + F5)    : <executable>
# CMake:Debug                               (Ctrl + F5)
