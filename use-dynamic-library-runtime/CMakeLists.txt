cmake_minimum_required(VERSION 3.25)
project(Runnable)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/lib)
file(MAKE_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY} ${CMAKE_LIBRARY_OUTPUT_DIRECTORY} ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY})


# Socket library (include directory & pre-built library binaries)
set(LIBSOCKET_INCLUDE_DIRECTORIES external/Socket/include)
set(LIBSOCKET_LIBRARY_DIRECTORIES external/Socket/build/lib) # contains both static & shared libraries



# Runnable (actual executable with linked libraries at compile time)
set(SOURCES main.cpp)
set(TARGET Runnable_executable)
set(TARGET_LIBRARIES Socket) # automatically uses shared version of the library, if not specifically mentioned (i.e. Socket.a)
add_executable(${TARGET} ${SOURCES})

# copy over pre-built libraries to the build folder
add_custom_command(TARGET ${TARGET} PRE_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${PROJECT_SOURCE_DIR}/${LIBSOCKET_LIBRARY_DIRECTORIES}/*"
        ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}
)

target_include_directories(${TARGET} PRIVATE ${LIBSOCKET_INCLUDE_DIRECTORIES})
target_link_directories(${TARGET} PRIVATE ${CMAKE_LIBRARY_OUTPUT_DIRECTORY})
target_link_libraries(${TARGET} ${TARGET_LIBRARIES})
set_target_properties(
    ${TARGET}
    PROPERTIES
    OUTPUT_NAME ${PROJECT_NAME}
)
# program needs to be ran from within the `./build/bin/` directory
# as the runtime loading of library is relative to tha path of the built binary
# (i.e. `dlopen('lib/libSocket.so', RTLD_LAZY)` inside the `build/bin` directory)
# build/bin
# |-- Runnable
# `-- lib
#     |-- libSocket.a
#     `-- libSocket.so
## this is common use case, just like a built (installed) game has all its libraries inside one `bin/lib` directory
## with the actual game binary in the `bin/` directory, and a shortcut to that binary is set to the desktop
