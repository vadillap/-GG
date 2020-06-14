cmake_minimum_required(VERSION 3.15)
project(cgraph)

set(CMAKE_CXX_STANDARD 20)

add_executable(cgraph main.cpp PnmFile.cpp PnmFile.h PnmUtils.cpp PnmUtils.h PnmColor.cpp PnmColor.h)