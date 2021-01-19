set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/cmake)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

if (Threads_FOUND)
    add_definitions(-D_HAVE_PTHREAD_)
endif (Threads_FOUND)

if (NOT WITHOUT_GEOS)
    find_package(GEOS)
else ()
    message("GEOS disabled")
endif ()

if (GEOS_FOUND)
    add_definitions(-DHAVE_GEOS)
endif ()

if (NOT WITHOUT_TEST)
    find_package(Catch2)
    if (NOT Catch2_FOUND)
        MESSAGE(WARNING "Catch2 not found, disabling tests")
    endif ()
endif ()
