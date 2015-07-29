cmake_minimum_required(VERSION 3.0)

set(SOURCE_FILES main.cpp)
set(HEADER_FILES
    app.hpp
    login_server_proxy.hpp
    logic_system.hpp
)

add_executable(account_server ${SOURCE_FILES} ${HEADER_FILES})
install(TARGETS account_server DESTINATION .)
