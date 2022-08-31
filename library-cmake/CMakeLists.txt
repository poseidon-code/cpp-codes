cmake_minimum_required(VERSION 3.23)
project(simplemath)

set(CMAKE_CXX_STANDARD 20)

file(GLOB_RECURSE SOURCES src/*.cpp)
add_library(simplemath STATIC ${SOURCES})
target_include_directories(simplemath PRIVATE include)
