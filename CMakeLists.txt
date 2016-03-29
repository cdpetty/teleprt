cmake_minimum_required(VERSION 3.3)
project(teleprt)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "~/Documents/Git/teleprt/Binaries")

set(SOURCE_FILES
    teleprt.c)

add_executable(teleprt ${SOURCE_FILES})