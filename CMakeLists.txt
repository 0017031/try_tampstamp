cmake_minimum_required(VERSION 3.12)

project(cel-retry LANGUAGES CXX)
add_executable(cel-retry
        src/cel-retry.cpp)

target_include_directories(cel-retry PRIVATE include)

target_compile_features(cel-retry PRIVATE cxx_std_98)
if (MSVC)
    target_compile_options(cel-retry PRIVATE /W4 /WX)
else (MSVC)
    target_compile_options(cel-retry PRIVATE -Wno-deprecated-declarations) # -Werror -Wall -Wextra -pedantic
endif (MSVC)


#set_target_properties(cel-retry
#    PROPERTIES
#    ARCHIVE_OUTPUT_DIRECTORY "/home/xy/cmake_practice/lib/"
#    LIBRARY_OUTPUT_DIRECTORY "/home/xy/cmake_practice/lib/"
#    RUNTIME_OUTPUT_DIRECTORY "/home/xy/cmake_practice/bin/"
#)