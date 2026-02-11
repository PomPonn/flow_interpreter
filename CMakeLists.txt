cmake_minimum_required(VERSION 3.10.0)
project(FlowInterpreter VERSION 0.1.0 LANGUAGES C)

set(TEST_NAME operators)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/src/*.c)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)

add_compile_definitions(
    _INCLUDE_PROFILER=1
    _INCLUDE_INLINE_LOGS=1
    _SUPPRESS_ERRORS=0
    _TEST_INPUT_PATH="${CMAKE_CURRENT_SOURCE_DIR}/src/tests/input/${TEST_NAME}.fl"
    _PROFILER_LOGS_PATH="${CMAKE_CURRENT_SOURCE_DIR}/logs/profiler.txt"
)

add_library(FlowLibrary STATIC ${SOURCES})
add_executable(Test ${CMAKE_CURRENT_SOURCE_DIR}/src/tests/main.c)
target_link_libraries(Test FlowLibrary)
