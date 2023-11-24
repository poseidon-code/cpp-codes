cmake_minimum_required(VERSION 3.25)
project(run)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(1D_super_sampling 1D/super_sampling.cpp)
add_executable(1D_sub_sampling 1D/sub_sampling.cpp)
add_executable(1D_patch 1D/patch.cpp)
add_executable(interpolation interpolation.cpp)
