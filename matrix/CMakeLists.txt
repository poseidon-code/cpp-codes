cmake_minimum_required(VERSION 3.25)
project(run)

set(CXX_STANDARD 20)
set(CXX_STANDARD_REQUIRED ON)

add_executable(1D_super_sampling 1D/super_sampling.cpp)
add_executable(interpolation interpolation.cpp)
