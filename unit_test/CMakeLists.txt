cmake_minimum_required (VERSION 3.1)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)


project (cel_try_unit_test LANGUAGES CXX)

add_executable(cel_try_unit_test test_main.cpp test_Timestamp.cpp)
include_directories(../include C:/Users/baic/Downloads/don_not_sync/date_howardHinnant/include/date C:/Users/baic/Downloads/don_not_sync/Catch2_c++_test_Freamwork/single_include/catch2)

target_sources(cel_try_unit_test PRIVATE ../include/Timestamp.h)

add_test(test_main.cpp test_Timestamp.cpp)

