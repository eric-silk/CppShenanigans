cmake_minimum_required(VERSION 3.14)
project(bgl_tests)
set(CMAKE_CXX_STANDARD 17)

find_package(Boost 1.74 COMPONENTS graph REQUIRED)

add_executable(bgl_test bgl_test.cpp)
target_link_libraries(bgl_test Boost::graph)

add_executable(iostream_test iostream_test.cpp)
add_executable(iostream_deque_test iostream_deque_test.cpp)
