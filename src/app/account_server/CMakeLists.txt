cmake_minimum_required(VERSION 3.0)

set(SOURCE_FILES main.cpp)
set(HEADER_FILES  )

add_executable(account_server ${SOURCE_FILES} ${HEADER_FILES})
install(TARGETS account_server DESTINATION .)
