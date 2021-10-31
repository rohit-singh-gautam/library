cmake_minimum_required(VERSION 3.18)

set(include_common
    ${CMAKE_SOURCE_DIR}/include
)

project(TestLibraryTries VERSION 1.0)
add_executable(TestLibraryTries tries.cc)
include_directories(TestLibraryTries PUBLIC ${include_common})

project(TestLibraryTree VERSION 1.0)
add_executable(TestLibraryTree tree.cc)
include_directories(TestLibraryTree PUBLIC ${include_common})