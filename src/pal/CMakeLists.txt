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
        util.cpp
        )

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

set(PUB_HEADERS
        ${CMAKE_SOURCE_DIR}/src/includes/pal/label.h
        ${CMAKE_SOURCE_DIR}/src/includes/pal/layer.h
        ${CMAKE_SOURCE_DIR}/src/includes/pal/pal.h
        ${CMAKE_SOURCE_DIR}/src/includes/pal/palexception.h
        ${CMAKE_SOURCE_DIR}/src/includes/pal/palgeometry.h
        ${CMAKE_SOURCE_DIR}/src/includes/pal/palstat.h
        )

if (GEOS_FOUND)
    LIST(APPEND SOURCES GeosWrapGeometry.cpp)
    LIST(APPEND PUB_HEADERS ${CMAKE_SOURCE_DIR}/src/includes/pal/GeosWrapGeometry.h)
endif ()


add_library(pal SHARED ${SOURCES} ${HEADERS} ${PUB_HEADERS})

set_target_properties(pal PROPERTIES PUBLIC_HEADERS "${PUB_HEADERS}")

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

install(TARGETS pal
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
        )

install(DIRECTORY "${CMAKE_SOURCE_DIR}/src/includes/pal"
        DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}"
        FILES_MATCHING
        PATTERN "*.h"
        )