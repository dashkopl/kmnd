cmake_minimum_required(VERSION 2.8)

get_filename_component(_path deps/googletest/googletest ABSOLUTE)

if(EXISTS ${_path}/CMakeLists.txt)

add_subdirectory(deps/googletest/googletest EXCLUDE_FROM_ALL)

add_executable(kmnd_tests EXCLUDE_FROM_ALL
        src/malloc.c
        src/malloc.h
        src/command.cpp
        src/error.cpp
        src/input.cpp
        src/option_boolean.cpp
        src/option_double.cpp
        src/option_float.cpp
        src/option_int8.cpp
        src/option_int16.cpp
        src/option_int32.cpp
        src/option_int64.cpp
        src/option_string.cpp
        src/option_uint8.cpp
        src/option_uint16.cpp
        src/option_uint32.cpp
        src/option_uint64.cpp
        src/path.cpp
        src/terminal.cpp
        src/usage.cpp)

target_include_directories(kmnd_tests PUBLIC
                           deps/googletest/googletest/include)

target_link_libraries(kmnd_tests kmnd dl)
target_link_libraries(kmnd_tests gtest gtest_main)

endif()
