cmake_minimum_required(VERSION 3.25)
project(Runnable)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)


# Socket library (creates both static & shared libraries, but automatically uses shared version)
set(LIBSOCKET_INCLUDE_DIRECTORIES external/Socket/include)
set(LIBSOCKET_SOURCES external/Socket/src/Socket.cpp)

# libSocket.a (should be explicitly mentioned for using)
add_library(Socket_static STATIC ${LIBSOCKET_SOURCES})
target_include_directories(Socket_static PUBLIC ${LIBSOCKET_INCLUDE_DIRECTORIES})
set_target_properties(
    Socket_static
    PROPERTIES
    OUTPUT_NAME Socket
)

# libSocket.so (automatically used if not mentioned otherwise)
add_library(Socket_shared SHARED ${LIBSOCKET_SOURCES})
target_include_directories(Socket_shared PUBLIC ${LIBSOCKET_INCLUDE_DIRECTORIES})
set_target_properties(
    Socket_shared
    PROPERTIES
    OUTPUT_NAME Socket
)


# Runnable (actual executable with linked libraries)
set(LINK_LIBRARIES Socket) # automatically uses dynamic library unless mentioned, like - Socket.a (uses static version of the library)

add_executable(Runnable_executable main.cpp)
target_include_directories(Runnable_executable PRIVATE ${LIBSOCKET_INCLUDE_DIRECTORIES})
target_link_directories(Runnable_executable PRIVATE ${CMAKE_LIBRARY_OUTPUT_DIRECTORY})
target_link_libraries(Runnable_executable ${LINK_LIBRARIES})
set_target_properties(
    Runnable_executable
    PROPERTIES
    OUTPUT_NAME ${PROJECT_NAME}
)
