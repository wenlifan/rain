cmake_minimum_required(VERSION 3.0)

set(CMAKE_INSTALL_PREFIX ${CMAKE_SOURCE_DIR}/rain)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -Wall -Wextra -pthread")
#set(CMAKE_CXX_FLAGS_DEBUG "-g")
#set(CMAKE_CXX_FLAGS_RELEASE "-O3")

set(ASIO_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/third_party/asio)
set(LUA_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/third_party/lua)
set(RAIN_SHARED_DIR ${CMAKE_SOURCE_DIR}/src/shared)

include_directories(${ASIO_INCLUDE_DIR} ${LUA_INCLUDE_DIR} ${RAIN_SHARED_DIR})
add_definitions(-DASIO_STANDALONE)
link_libraries(lua)

if(CMAKE_SYSTEM MATCHES "MSYS|CYGWIN")
    add_definitions(-D__USE_W32_SOCKETS -D_WIN32_WINNT=0x0601)
    link_libraries(ws2_32 wsock32)
endif()

add_subdirectory(src rain)

install(DIRECTORY templates/ DESTINATION .)

add_custom_target(debug
	COMMAND ${CMAKE_COMMAND} -DCMAKE_BUILD_TYPE=Debug ${CMAKE_SOURCE_DIR}
	COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR} --target all
	COMMENT "Build rain with cmake Debug mode."
)

add_custom_target(release
	COMMAND ${CMAKE_COMMAND} -DCMAKE_BUILD_TYPE=Release ${CMAKE_SOURCE_DIR}
	COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR} --target all
	COMMENT "Build rain with cmake Release mode."
)

add_custom_target(release_with_debug_info
	COMMAND ${CMAKE_COMMAND} -DCMAKE_BUILD_TYPE=RelWithDebInfo ${CMAKE_SOURCE_DIR}
	COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR} --target all
	COMMENT "Build rain with cmake RelWithDebInfo mode."
)

if(${CMAKE_BUILD_TYPE} MATCHES "Debug")
	message(STATUS "Compiler build flags: ${CMAKE_CXX_FLAGS} ${CMAKE_CXX_FLAGS_DEBUG}")
elseif(${CMAKE_BUILD_TYPE} MATCHES "Release")
	message(STATUS "Compiler build flags: ${CMAKE_CXX_FLAGS} ${CMAKE_CXX_FLAGS_RELEASE}")
elseif(${CMAKE_BUILD_TYPE} MATCHES "RelWithDebInfo")
	message(STATUS "Compiler build flags: ${CMAKE_CXX_FLAGS} ${CMAKE_CXX_FLAGS_RELWITHDEBINFO}")
endif()

