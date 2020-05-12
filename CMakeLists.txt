cmake_minimum_required(VERSION 3.16)
project(try_timestamp)

set(CMAKE_CXX_STANDARD 20)

project(cel-retry LANGUAGES CXX)
add_executable(cel-retry
        cel-retry.cpp
        Timestamp.h
        )

target_include_directories(cel-retry PRIVATE include)

if (MSVC)
    target_compile_options(cel-retry PRIVATE /W4 /WX)
else (MSVC)
    target_compile_options(cel-retry PRIVATE -Wno-deprecated-declarations) # -Werror -Wall -Wextra -pedantic
endif (MSVC)


add_subdirectory(_unit_test EXCLUDE_FROM_ALL)
