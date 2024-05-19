cmake_minimum_required(VERSION 3.25)
project(shm)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

add_executable(write write.cpp)
target_include_directories(write PUBLIC ${CMAKE_SOURCE_DIR})

add_executable(read read.cpp)
target_include_directories(read PUBLIC ${CMAKE_SOURCE_DIR})
