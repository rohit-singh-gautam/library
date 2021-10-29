cmake_minimum_required(VERSION 3.18)

project(TestLibrary VERSION 1.0)

add_executable(TestLibrary
    tries.cc
)

include_directories(TestLibrary PUBLIC 
    ${CMAKE_SOURCE_DIR}/include
)