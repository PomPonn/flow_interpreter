cmake_minimum_required(VERSION 4.0.0)
project(FlowInterpreter VERSION 0.1.1 LANGUAGES C)

set(test_name simple)
set(modules_base_path ${CMAKE_CURRENT_SOURCE_DIR}/modules)

add_library(flow STATIC)

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

link_libraries(flow)


function(build_module_dlls)
    file(GLOB module_paths CONFIGURE_DEPENDS ${modules_base_path}/*/)
    list(FILTER module_paths EXCLUDE REGEX "\\..*$")
    foreach(module_path ${module_paths})
        cmake_path(GET module_path FILENAME module_name)
        file(GLOB_RECURSE module_sources CONFIGURE_DEPENDS ${module_path}/*.c)
        if (module_sources)
            add_library(${module_name} SHARED ${module_sources})
            target_include_directories(${module_name} PRIVATE ${module_path})
        endif()
    endforeach()
endfunction()

build_module_dlls()

# gather source files
file(GLOB_RECURSE flow_sources CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/src/*.c)

add_compile_definitions(
    _INCLUDE_PROFILER=1
    _INCLUDE_INLINE_LOGS=0
    _SUPPRESS_ERRORS=0
    _TEST_INPUT_PATH="${CMAKE_CURRENT_SOURCE_DIR}/test/input/${test_name}.fl"
    _PROFILER_LOGS_PATH="${CMAKE_CURRENT_SOURCE_DIR}/logs/profiler.txt"
)

target_sources(flow PRIVATE ${flow_sources})
add_executable(Test ${CMAKE_CURRENT_SOURCE_DIR}/test/main.c)