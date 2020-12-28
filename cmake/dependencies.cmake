set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/cmake)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
add_definitions(-D_HAVE_PTHREAD_)

find_package(GEOS)

if (GEOS_Found)
    add_definitions(-DHAVE_GEOS)
endif()
