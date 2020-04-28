cmake_minimum_required (VERSION 3.1)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CONFIGURATION_TYPES Debug;Release)

add_executable (file-mapping-with-boost file-mmap.cpp)
target_include_directories (file-mapping-with-boost
        PUBLIC
        C:/Users/baic/Downloads/DO_NOT_sync/ms-gsl/include)


#add_executable(try_boost.spirit.x3 try_boost.spirit.x3.cpp)
#target_include_directories (try_boost.spirit.x3
#        PUBLIC
#        F:/msys64/mingw64/include)
