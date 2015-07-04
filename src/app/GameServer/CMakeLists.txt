cmake_minimum_required(VERSION 3.0)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=c++14")

set(SHARED_DIR ../../shared)
include_directories(${SHARED_DIR})

set(SOURCE_FILES main.cpp)
set(HEADER_FILES  )

add_executable(GameServer ${SOURCE_FILES} ${HEADER_FILES})
