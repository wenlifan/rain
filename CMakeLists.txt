cmake_minimum_required(VERSION 3.0)

set(CMAKE_INSTALL_PREFIX ${CMAKE_SOURCE_DIR}/rain)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -Wall -Wextra -pthread")
#set(CMAKE_CXX_FLAGS_DEBUG "-g")
#set(CMAKE_CXX_FLAGS_RELEASE "-O3")

set(SHARED_DIR ${CMAKE_SOURCE_DIR}/src/shared)
set(ASIO_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/asio)
set(LUA_INCLUDE_DIR /usr/include/lua5.2)

include_directories(${ASIO_INCLUDE_DIR} ${LUA_INCLUDE_DIR} ${SHARED_DIR})
add_definitions(-DASIO_STANDALONE)

add_subdirectory(src rain)

install(DIRECTORY templates/ DESTINATION .)

add_custom_target(debug
	COMMAND ${CMAKE_COMMAND} -DCMAKE_BUILD_TYPE=Debug ${CMAKE_SOURCE_DIR}
	COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR} --target all
	COMMENT "build rain with debug."
)

add_custom_target(release
	COMMAND ${CMAKE_COMMAND} -DCMAKE_BUILD_TYPE=Release ${CMAKE_SOURCE_DIR}
	COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR} --target all
	COMMENT "build rain with release."
)