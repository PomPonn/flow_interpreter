cmake_minimum_required(VERSION 3.10.0)
project(InterpreterTools VERSION 0.1.0 LANGUAGES C)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/src/*.c)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/inc)

add_library(InterpreterTools STATIC ${SOURCES})

add_executable(Example ${CMAKE_CURRENT_SOURCE_DIR}/main.c)
target_link_libraries(Example InterpreterTools)