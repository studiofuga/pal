set(SOURCES pal_tests.cpp test_labelling.cpp)

if (GEOS_FOUND)
    LIST(APPEND SOURCES test_geos_labelling.cpp)
endif ()

add_executable(unittests ${SOURCES})

if (WITH_BENCHMARKS)
    target_compile_definitions(unittests -DHAVE_BENCHMARKS)
endif ()

target_link_libraries(unittests PRIVATE Catch2::Catch2 pal)

include(CTest)
include(Catch)
catch_discover_tests(unittests)
enable_testing()
