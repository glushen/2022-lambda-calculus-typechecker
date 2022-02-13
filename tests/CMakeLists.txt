project(Google_tests)
add_subdirectory(lib)
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

add_executable(
    Google_Tests_run
    tests.cpp
)

target_link_libraries(Google_Tests_run lambda_calculus_typechecker_lib)

target_link_libraries(Google_Tests_run gtest gtest_main)