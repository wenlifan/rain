cmake_minimum_required(VERSION 3.0)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -Wall -Wextra -pthread")

set(ASIO_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/third_party/asio)
set(LUA_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/third_party/lua)
set(RAIN_SHARED_DIR ${CMAKE_SOURCE_DIR}/src/shared)

include_directories(${ASIO_INCLUDE_DIR} ${LUA_INCLUDE_DIR} ${RAIN_SHARED_DIR})
add_definitions(-DASIO_STANDALONE)
link_libraries(lua_static)

if(CMAKE_SYSTEM MATCHES "MSYS|CYGWIN")
    add_definitions(-D__USE_W32_SOCKETS -D_WIN32_WINNT=0x0601)
    link_libraries(ws2_32 wsock32)
endif()

add_subdirectory(shared)
add_subdirectory(app)
add_subdirectory(test_all)

if(${CMAKE_BUILD_TYPE} MATCHES "Debug")
    message(STATUS "Rain compiler build flags: ${CMAKE_CXX_FLAGS} ${CMAKE_CXX_FLAGS_DEBUG}")
elseif(${CMAKE_BUILD_TYPE} MATCHES "Release")
    message(STATUS "Rain compiler build flags: ${CMAKE_CXX_FLAGS} ${CMAKE_CXX_FLAGS_RELEASE}")
elseif(${CMAKE_BUILD_TYPE} MATCHES "RelWithDebInfo")
    message(STATUS "Rain compiler build flags: ${CMAKE_CXX_FLAGS} ${CMAKE_CXX_FLAGS_RELWITHDEBINFO}")
endif()
