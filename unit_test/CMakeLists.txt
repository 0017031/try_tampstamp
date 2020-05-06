cmake_minimum_required(VERSION 3.1)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)


project(cel_try_unit_test LANGUAGES CXX)

add_executable(cel_try_unit_test
        test_main.cpp
        test_Timestamp.cpp)

target_include_directories(cel_try_unit_test PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/../include/)

#target_compile_options(cel_try_unit_test PRIVATE
##  $<$<CXX_COMPILER_ID:MSVC>:/W4 /WX>
#  $< $<NOT: $<CXX_COMPILER_ID:MSVC> >:-Wno-deprecated-declarations >
#)

target_compile_options(cel_try_unit_test PRIVATE
        $<$<CXX_COMPILER_ID:MSVC>:/W4 /WX>
        #  $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -pedantic -Werror>
        $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wno-deprecated-declarations>)

#target_sources(cel_try_unit_test PRIVATE ../include/Timestamp.h)

#add_test(test_main.cpp test_Timestamp.cpp)

