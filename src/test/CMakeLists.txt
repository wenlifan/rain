cmake_minimum_required(VERSION 3.0)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -Wall")
set(SHARED_DIR ../shared)
include_directories(${SHARED_DIR})

set(HEADER_FILES ${SHARED_DIR}/unittest.hpp)
file(GLOB_RECURSE SOURCE_FILES ${SHARED_DIR}/*_test.cpp)

add_executable(TestAll ${SOURCE_FILES} ${HEADER_FILES})
