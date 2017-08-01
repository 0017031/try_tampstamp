cmake_minimum_required (VERSION 3.1)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)


project (cel_try_unit_test)

add_executable(cel_try_unit_test test_main.cpp test_Timestamp.cpp)
include_directories(../include C:/Users/baic/Downloads/Work/github-libs/date C:/Users/baic/Downloads/work/github-libs/Catch/single_include)

target_sources(cel_try_unit_test PUBLIC ../include/Timestamp.h)

add_test(test_main.cpp test_Timestamp.cpp)

