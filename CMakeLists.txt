cmake_minimum_required(VERSION 3.12.0)
project(FlowInterpreter VERSION 0.1.1 LANGUAGES C)

set(test_name simple)
set(modules_base_path ${CMAKE_CURRENT_SOURCE_DIR}/modules)

function(build_module_dlls base_path)
    # match directories
    file(GLOB submodule_paths ${base_path}/*/)
    file(APPEND ${CMAKE_CURRENT_SOURCE_DIR}/tes.txt ${base_path})
    # match module sources
    file(GLOB module_sources RELATIVE base_path *.c)
    
    if(${module_sources})
        cmake_path(GET ${submodule_paths} FILENAME mod_name)
        add_library(${mod_name} MODULE ${module_sources})
    endif()

    foreach(path submodule_paths) 
        build_module_dlls(path)
    endforeach() 
     

    build_module_dlls()
endfunction()

build_module_dlls(${modules_base_path})

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
    _TEST_INPUT_PATH="${CMAKE_CURRENT_SOURCE_DIR}/test/input/${TEST_NAME}.fl"
    _PROFILER_LOGS_PATH="${CMAKE_CURRENT_SOURCE_DIR}/logs/profiler.txt"
)

add_library(FlowLibrary STATIC ${SOURCES})
add_executable(Test ${CMAKE_CURRENT_SOURCE_DIR}/test/main.c)
target_link_libraries(Test FlowLibrary)
