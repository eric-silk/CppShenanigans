cmake_minimum_required(VERSION 3.14)
project(buffer_tests)
set(CMAKE_CXX_STANDARD 17)

add_executable(buffer_test buffer_test.cpp)
add_executable(hton_ntoh_test hton_ntoh_test.cpp)