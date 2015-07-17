cmake_minimum_required(VERSION 3.0)

add_custom_target(rain_shared SOURCES
    filesystem.hpp
    singleton.hpp
    protocol.hpp
    message_pack.hpp
    server_node.hpp
    client_node.hpp
    session.hpp
    thread_safe_queue.hpp
    console.hpp
    config_reader.hpp
    lua_environment.hpp
)
