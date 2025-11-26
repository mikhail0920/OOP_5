cmake_minimum_required(VERSION 3.10)
project(Lab5)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(googletest)

enable_testing()

include_directories(src)

add_executable(lab_tests tests/tests.cpp)

target_link_libraries(lab_tests gtest gtest_main)