cmake_minimum_required(VERSION 3.10)
project(prao-compresser)
set(CMAKE_CXX_STANDARD 14)
add_executable(prao-compresser main.cpp Reader.cpp Reader.h FileHeader.cpp FileHeader.h)