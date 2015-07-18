cmake_minimum_required(VERSION 3.0)

file(GLOB_RECURSE ASIO_FILES asio/*.*)
set(ASIO_FILES ${ASIO_FILES} asio.hpp)

add_custom_target(asio_files SOURCES ${ASIO_FILES})
