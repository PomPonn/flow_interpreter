cmake_minimum_required(VERSION 3.12.0)
project(FlowInterpreter VERSION 0.1.1 LANGUAGES C)

set(test_name simple)
set(modules_base_path ${CMAKE_CURRENT_SOURCE_DIR}/modules)

macro(test_log rel_path log)
file(WRITE ${CMAKE_CURRENT_SOURCE_DIR}/test2.txt ${rel_path})
cmake_path(APPEND CMAKE_CURRENT_SOURCE_DIR ${rel_path} OUTPUT_VARIABLE final_path)
file(WRITE ${final_path} ${log})
endmacro()

# gather source files
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/src/*.c)
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/src/modules
)

add_compile_definitions(
    _INCLUDE_PROFILER=1
    _INCLUDE_INLINE_LOGS=0
    _SUPPRESS_ERRORS=0
    _TEST_INPUT_PATH="${CMAKE_CURRENT_SOURCE_DIR}/test/input/${test_name}.fl"
    _PROFILER_LOGS_PATH="${CMAKE_CURRENT_SOURCE_DIR}/logs/profiler.txt"
)

add_library(FlowLibrary STATIC ${SOURCES})
add_executable(Test ${CMAKE_CURRENT_SOURCE_DIR}/test/main.c)
target_link_libraries(Test FlowLibrary)
