cmake_minimum_required(VERSION 3.0)

include_directories(${PROJECT_SOURCE_DIR}/asio)
include_directories(/usr/include/lua5.2)

add_subdirectory(src rain)
