cmake_policy(SET CMP0048 NEW)

project(libpal VERSION 0.2 LANGUAGES CXX)

cmake_minimum_required(VERSION 3.17)

include(cmake/dependencies.cmake)

add_subdirectory(src/pal)
add_subdirectory(tests)

