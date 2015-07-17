cmake_minimum_required(VERSION 3.0)

set(HEADER_FILES)
file(GLOB_RECURSE SOURCE_FILES ${SHARED_DIR}/*_test.cpp)

add_executable(TestAll ${SOURCE_FILES} ${HEADER_FILES})
target_link_libraries(TestAll lua5.2)
install(TARGETS TestAll DESTINATION .)
