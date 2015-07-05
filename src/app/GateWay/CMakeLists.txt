cmake_minimum_required(VERSION 3.0)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=c++14")

add_definitions(-DASIO_STANDALONE)

set(SHARED_DIR ../../shared)
set(ASIO_DIR ${SHARED_DIR}/network)
include_directories(${SHARED_DIR} ${ASIO_DIR})

set(SOURCE_FILES main.cpp)
set(HEADER_FILES  )

add_executable(GateWay ${SOURCE_FILES} ${HEADER_FILES})
