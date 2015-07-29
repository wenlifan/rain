cmake_minimum_required(VERSION 3.0)

set(SOURCE_FILES main.cpp)
set(HEADER_FILES
    app.hpp
    account_server_proxy.hpp
    gateway_server_proxy.hpp
    logic_system.hpp
)

add_executable(login_server ${SOURCE_FILES} ${HEADER_FILES})
install(TARGETS login_server DESTINATION .)
