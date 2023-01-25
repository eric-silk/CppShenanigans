cmake_minimum_required(VERSION 3.14)
project(buffer_tests)
set(CMAKE_CXX_STANDARD 17)

add_executable(deleted_ctor_test deleted_ctor_test.cpp)
add_executable(derived_copy_ctor_test derived_copy_ctor_test.cpp)