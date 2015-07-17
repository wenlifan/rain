cmake_minimum_required(VERSION 3.0)

set(SOURCE_FILES main.cpp)
set(HEADER_FILES  )

add_executable(DataServer ${SOURCE_FILES} ${HEADER_FILES})
target_link_libraries(DataServer lua5.2)
install(TARGETS DataServer DESTINATION .)
