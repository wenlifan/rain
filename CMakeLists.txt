cmake_minimum_required(VERSION 3.0)

set(CMAKE_EXPORT_COMPILE_COMMANDS 1)
set(CMAKE_INSTALL_PREFIX ${CMAKE_SOURCE_DIR}/server)

add_subdirectory(third_party)
add_subdirectory(src rain)

file(GLOB_RECURSE TEMPLATE_FILES templates/*.*)
add_custom_target(template_files SOURCES ${TEMPLATE_FILES})

install(DIRECTORY templates/ DESTINATION .)

add_custom_target(debug
    COMMAND ${CMAKE_COMMAND} -DCMAKE_BUILD_TYPE=Debug ${CMAKE_SOURCE_DIR}
    COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR} --target all
    COMMENT "Build rain with cmake Debug mode."
)

add_custom_target(release
    COMMAND ${CMAKE_COMMAND} -DCMAKE_BUILD_TYPE=Release ${CMAKE_SOURCE_DIR}
    COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR} --target all
    COMMENT "Build rain with cmake Release mode."
)

add_custom_target(release_with_debug_info
    COMMAND ${CMAKE_COMMAND} -DCMAKE_BUILD_TYPE=RelWithDebInfo ${CMAKE_SOURCE_DIR}
    COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR} --target all
    COMMENT "Build rain with cmake RelWithDebInfo mode."
)
