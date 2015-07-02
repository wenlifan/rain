cmake_minimum_required(VERSION 3.0)

include_directories(../shared)

set(CMAKE_CXX_FLAGS "-std=c++14")

set(SOURCE_FILES ../shared/unittest.cpp)
set(HEADER_FILES  )

add_executable(TestAll ${SOURCE_FILES} ${HEADER_FILES})