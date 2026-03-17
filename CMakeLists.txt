cmake_minimum_required(VERSION 4.0.0)
project(FlowInterpreter VERSION 0.1.2 LANGUAGES C)

# ---- GLOBAL VARIABLES -----

# base directory for language modules
set(MODULES_DIR ${CMAKE_CURRENT_SOURCE_DIR}/modules)
# path to the sample flow code to use
set(SAMPLE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/test/samples/simple.fl)

# ---- FUNCTIONS ----

# add every top level module in MODULES_DIR as separate shared library
function(add_module_libs)
    file(GLOB module_paths CONFIGURE_DEPENDS ${MODULES_DIR}/*/)
    # exclude elements with an extension
    list(FILTER module_paths EXCLUDE REGEX "\\..*$")
    
    foreach(module_path ${module_paths})
        cmake_path(GET module_path FILENAME module_name)
        # gather all module sources
        file(GLOB_RECURSE module_sources CONFIGURE_DEPENDS ${module_path}/*.c)
        # add library target if there are any sources
        if (module_sources)
            add_library(${module_name} SHARED ${module_sources})
            target_include_directories(${module_name} PRIVATE ${module_path})
        endif()
    endforeach()
endfunction()

# ---- TARGETS ----

add_library(flow STATIC)
link_libraries(flow)
add_module_libs()
add_executable(test)

# ---- CONFIG ----

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

add_compile_definitions(
    _INCLUDE_PROFILER=1
    _INCLUDE_INLINE_LOGS=0
    _SUPPRESS_ERRORS=0
    _TEST_INPUT_PATH="${SAMPLE_PATH}"
    _PROFILER_LOGS_PATH="${CMAKE_CURRENT_SOURCE_DIR}/logs/profiler.txt"
)

# ---- SOURCES ----

# gather main source files
file(GLOB_RECURSE flow_sources CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/src/*.c)

# add sources
target_sources(flow PRIVATE ${flow_sources})
target_sources(test PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/test/main.c)