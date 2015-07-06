cmake_minimum_required(VERSION 3.0)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -pthread -std=c++14")

add_definitions(-DASIO_STANDALONE)

set(SHARED_DIR ../../shared)
set(NETWORK_DIR ${SHARED_DIR}/network)
include_directories(${SHARED_DIR} ${NETWORK_DIR})

set(SOURCE_FILES main.cpp)
set(HEADER_FILES  )

add_executable(GateWay ${SOURCE_FILES} ${HEADER_FILES})
