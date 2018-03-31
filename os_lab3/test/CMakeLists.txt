cmake_minimum_required(VERSION 3.9)
project(HHOS)

set(CMAKE_CXX_STANDARD 11)
include_directories(/usr/include)
add_executable(TEST
    libc/test.cpp kernel/sh.h)
