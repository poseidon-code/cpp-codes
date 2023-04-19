cmake_minimum_required(VERSION 3.25)
project(SocketTransmitter)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SOURCES
    src/Socket.cpp
)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)


if(CMAKE_BUILD_TYPE STREQUAL "Release")
    # libSocket.a
    add_library(Socket_static STATIC ${SOURCES})
    target_include_directories(Socket_static PUBLIC include)
    set_target_properties(
        Socket_static
        PROPERTIES 
        OUTPUT_NAME Socket
    )

    # libSocket.so
    add_library(Socket_shared SHARED ${SOURCES})
    target_include_directories(Socket_shared PUBLIC include)
    set_target_properties(
        Socket_shared
        PROPERTIES 
        OUTPUT_NAME Socket
    )
endif()

# SocketTransmitter
add_executable(SocketTransmitter_executable SocketTransmitter.cpp ${SOURCES})
target_include_directories(SocketTransmitter_executable PUBLIC include)
set_target_properties(
    SocketTransmitter_executable
    PROPERTIES
    OUTPUT_NAME ${PROJECT_NAME}
)


# VS Code CMake Tools Commands
# CMake:Configure                                           : cmake -S . -B build
# CMake:Delete Cache and reconfigure                        : cmake --fresh -S . -B build
# CMake:Clean Rebuild                                       : cmake --build . --target clean; cmake --build . --target all
# CMake:Build Target                                        : cmake --build . --target <target-name>
# CMake:Build                               (F7)            : cmake --build . --target all
# CMake:Run without Debugging               (Shift + F5)    : <executable>
# CMake:Debug                               (Ctrl + F5)
