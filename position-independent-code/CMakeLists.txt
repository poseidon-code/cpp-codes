cmake_minimum_required(VERSION 3.25)
project(pic)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)


add_library(hello_static STATIC hello/hello.cpp)
target_include_directories(hello_static PUBLIC hello/)
set_target_properties(hello_static
    PROPERTIES
    OUTPUT_NAME hello
    POSITION_INDEPENDENT_CODE ON
)


add_library(hello_shared SHARED hello/hello.cpp)
target_include_directories(hello_shared PUBLIC hello/)
set_target_properties(hello_shared
    PROPERTIES
    OUTPUT_NAME hello
    POSITION_INDEPENDENT_CODE ON
)


add_executable(run_static main.cpp)
target_include_directories(run_static PUBLIC hello/)
target_link_directories(run_static PUBLIC ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY})
target_link_libraries(run_static hello.a)



add_executable(run_shared main.cpp)
target_include_directories(run_shared PUBLIC hello/)
target_link_directories(run_shared PUBLIC ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY})
target_link_libraries(run_shared hello.so)
