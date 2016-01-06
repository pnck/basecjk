cmake_minimum_required(VERSION 3.3)
project(baseCJK)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
#add_definitions(-D_USEASM)
set(SOURCE_FILES main.cpp basecjk.cpp basecjk.h)
add_executable(baseCJK ${SOURCE_FILES})