cmake_minimum_required(VERSION 3.9)
project(nparray CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)

add_library(nparray STATIC src/npy.cpp src/nparray.cpp)
