cmake_minimum_required(VERSION 3.0)

set(SOURCE_FILES main.cpp)
set(HEADER_FILES
    app.hpp
    server_session_manager.hpp
    gateway_proxy.hpp
    data_server_proxy.hpp
    shared_server_proxy.hpp
)

add_executable(game_server ${SOURCE_FILES} ${HEADER_FILES})
install(TARGETS game_server DESTINATION .)