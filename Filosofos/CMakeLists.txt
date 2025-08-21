cmake_minimum_required(VERSION 3.31.6)

project(Filosofos)

set(CMAKE_CXX_STANDARD 26)

include_directories(..)

add_executable(${PROJECT_NAME} main.cpp)
