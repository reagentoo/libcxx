cmake_minimum_required (VERSION 2.8) 
project (libcxx)

list(APPEND CMAKE_CXX_FLAGS "-std=c++1y")

include_directories(include)

add_executable (libcxx tests/tuple.cpp)
