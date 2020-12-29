cmake_policy(SET CMP0048 NEW)

project(libpal VERSION 1.0 LANGUAGES CXX)

cmake_minimum_required(VERSION 3.10)

set (CMAKE_CXX_STANDARD 14)

include(cmake/options.cmake)
include(cmake/dependencies.cmake)
include(CPack)
include(GNUInstallDirs)

add_subdirectory(src/pal)
add_subdirectory(tests)

