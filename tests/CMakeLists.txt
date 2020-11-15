add_executable(pal_labeller
        Geom.cpp Geom.h
        pal.cpp
        )


target_include_directories(pal_labeller
        PUBLIC
        ${CMAKE_SOURCE_DIR}/src/includes
        PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}
        )

target_link_libraries(pal_labeller PRIVATE pal Threads::Threads)
