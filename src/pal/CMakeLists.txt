set(SOURCES
        feature.cpp
        geomfunction.cpp
        label.cpp
        labelposition.cpp
        layer.cpp
        pal.cpp
        palstat.cpp
        pointset.cpp
        priorityqueue.cpp
        problem.cpp
        util.cpp)


set(HEADERS
        rtree.hpp
        linkedlist.hpp
        hashtable.hpp
        feature.h
        geomfunction.h
        internalexception.h
        labelposition.h
        pointset.h
        priorityqueue.h
        problem.h
        simplemutex.h
        util.h)

add_library(pal ${SOURCES} ${HEADERS})

target_include_directories(pal
        PUBLIC
        ${CMAKE_SOURCE_DIR}/src/includes
        ${GEOS_INCLUDE_DIR}
        PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}
        )

target_link_libraries(pal
        PRIVATE
        Threads::Threads
        PUBLIC
        ${GEOS_LIBRARY}
        )
