add_executable(unittests
        pal_tests.cpp
        test_geos_labelling.cpp)

target_link_libraries(unittests PRIVATE Catch2::Catch2 pal)

include(CTest)
include(Catch)
catch_discover_tests(unittests)
enable_testing()
