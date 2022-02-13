cmake_minimum_required(VERSION 3.13)

project(lambda_calculus_typechecker_run)

set(CMAKE_CXX_STANDARD 20)

add_executable(lambda_calculus_typechecker_run main.cpp)

include_directories(src)
add_subdirectory(src)

target_link_libraries(lambda_calculus_typechecker_run lambda_calculus_typechecker_lib)

add_subdirectory(tests)
