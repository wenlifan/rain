cmake_minimum_required(VERSION 3.0)

set(SOURCE_FILES main.cpp)
set(HEADER_FILES
    app.hpp
    game_server_proxy.hpp
    shared_server_proxy.hpp
    logic_system.hpp
)

add_executable(data_server ${SOURCE_FILES} ${HEADER_FILES})
install(TARGETS data_server DESTINATION .)
