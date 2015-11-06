cmake_minimum_required(VERSION 2.8)
project(kmnd)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

include_directories(include)

set(SOURCE_FILES
    include/kmnd.h
    src/command.c
    src/command.h
    src/core.h
    src/error.c
    src/error.h
    src/input.c
    src/input.h
    src/kmnd.c
    src/option.c
    src/option.h
    src/path.c
    src/path.h
    src/terminal.c
    src/terminal.h
    src/usage.c
    src/usage.h)

add_library(kmnd ${SOURCE_FILES})

add_executable(sample EXCLUDE_FROM_ALL sample.c)
target_link_libraries(sample kmnd)

add_subdirectory(tests)
