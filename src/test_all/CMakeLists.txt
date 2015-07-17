cmake_minimum_required(VERSION 3.0)

set(HEADER_FILES)
file(GLOB_RECURSE SOURCE_FILES ${RAIN_SHARED_DIR}/*_test.cpp)

add_executable(test_all ${SOURCE_FILES} ${HEADER_FILES})
install(TARGETS test_all DESTINATION .)
