cmake_minimum_required(VERSION 3.16)
set(CMAKE_CXX_STANDARD 17)

project(cel_try_unit_test LANGUAGES CXX)
add_library(test_main_lib STATIC test_main.cpp)
add_executable(cel_try_unit_test test_Timestamp.cpp)

target_link_libraries(cel_try_unit_test test_main_lib)
target_compile_definitions(cel_try_unit_test PRIVATE 
	CATCH_CONFIG_FAST_COMPILE 
	WIN32_LEAN_AND_MEAN 
	VC_EXTRALEAN 
	NOMINMAX
	) #CATCH_CONFIG_DISABLE_MATCHERS
target_precompile_headers(cel_try_unit_test PRIVATE ../include/catch2_single_file.hpp)
#http://mochan.info/c++/2019/11/12/pre-compiled-headers-gcc-clang-cmake.html

target_include_directories(test_main_lib PUBLIC ../include)



target_include_directories(cel_try_unit_test PUBLIC
        ..
        ../include/
#        lib/catch2/include
        )

target_compile_options(cel_try_unit_test PRIVATE
$<$<CXX_COMPILER_ID:MSVC>: /EHsc > # /W4 /WX
$<$<NOT:$<CXX_COMPILER_ID:MSVC>>: -Wno-deprecated-declarations -Wno-integer-overflow -Wno-unknown-pragmas -Wall -Wextra -pedantic>) # 



#get_target_property(dirs cel_try_unit_test INCLUDE_DIRECTORIES)
#foreach(dir ${dirs})
#  message(STATUS "dir='${dir}'")
#endforeach()

#list(APPEND CMAKE_PREFIX_PATH "f:/lib")
#find_package(Catch2 REQUIRED)
#target_link_libraries(cel_try_unit_test Catch2::Catch2)

#include(CTest)
##include(Catch)
#catch_discover_tests(cel_try_unit_test)
#enable_testing()
#add_test(test_main.cpp test_Timestamp.cpp)

