cmake_minimum_required(VERSION 3.10.0)
project(FlowInterpreter VERSION 0.1.0 LANGUAGES C)

set(TEST_NAME main)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/src/*.c)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)

add_compile_definitions(
    _INCLUDE_PROFILER=1
    _SUPPRESS_ERRORS=0
    _PROJECT_DIR="${CMAKE_CURRENT_SOURCE_DIR}"
    _PROFILER_LOGS_PATH="${CMAKE_CURRENT_SOURCE_DIR}/logs/profiler.txt"
)

add_library(FlowInterpreter STATIC ${SOURCES})
add_executable(Example ${CMAKE_CURRENT_SOURCE_DIR}/tests/${TEST_NAME}.c)
target_link_libraries(Example FlowInterpreter)
