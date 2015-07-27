cmake_minimum_required(VERSION 3.0)

set(SOURCE_FILES main.cpp)
set(HEADER_FILES
    app.hpp
    game_server_proxy.hpp
    login_server_proxy.hpp
)

add_executable(gateway_server ${SOURCE_FILES} ${HEADER_FILES})
install(TARGETS gateway_server DESTINATION .)