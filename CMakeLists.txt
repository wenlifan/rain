cmake_minimum_required(VERSION 3.0)

set(SHARED_DIR ${CMAKE_SOURCE_DIR}/src/shared)
set(ASIO_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/asio)
set(LUA_INCLUDE_DIR /usr/include/lua5.2)

include_directories(${ASIO_INCLUDE_DIR} ${LUA_INCLUDE_DIR} ${SHARED_DIR})

add_definitions(-DASIO_STANDALONE)

set(CMAKE_INSTALL_PREFIX ${CMAKE_SOURCE_DIR}/rain)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -Wall -pthread")
#set(CMAKE_CXX_FLAGS_DEBUG "-g")
#set(CMAKE_CXX_FLAGS_RELEASE "-O3")

add_subdirectory(src rain)

add_custom_target(ProjectFiles)
install(DIRECTORY templates/ DESTINATION .)
