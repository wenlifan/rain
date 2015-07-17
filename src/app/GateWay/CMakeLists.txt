cmake_minimum_required(VERSION 3.0)

set(SOURCE_FILES main.cpp)
set(HEADER_FILES  )

add_executable(GateWay ${SOURCE_FILES} ${HEADER_FILES})
target_link_libraries(GateWay lua5.2)
install(TARGETS GateWay DESTINATION .)