cmake_minimum_required (VERSION 3.1)

# https://github.com/sakra/cotire
#set (CMAKE_MODULE_PATH "W:/github-libs/cotire/CMake")
#include(cotire)

add_executable (file-mapping-with-boost file-mapping-with-boost.cpp)
# target_include_directories (cs_string_tst PUBLIC C:/Users/baic/Downloads/work/github-libs/cs_string/src)

# To use an existing manually maintained prefix header instead of the automatically generated one, 
# set the COTIRE_CXX_PREFIX_HEADER_INIT property before invoking cotire:
#set_target_properties(cs_string_tst PROPERTIES COTIRE_CXX_PREFIX_HEADER_INIT "stdafx.h")
#cotire(cs_string_tst)

