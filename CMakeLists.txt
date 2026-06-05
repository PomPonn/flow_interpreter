cmake_minimum_required(VERSION 4.0.0)
project(FlowInterpreter VERSION 0.1.2 LANGUAGES C)

include(CTest)

# ---------- GLOBAL VARIABLES -----------

# base directory for dynamic language modules
set(MODULES_DIR ${CMAKE_CURRENT_SOURCE_DIR}/modules)
# base directory for unit tests
set(TESTS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/tests)
# base directory to core functionality
set(CORE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
# base directory for logs
set(LOG_DIR ${CMAKE_CURRENT_SOURCE_DIR}/logs)
# base directory to third party libraries
set(LIBS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/libs)
# path to the sample flow code to use
set(SAMPLE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/samples/calc.fl)
# path to unity library source file
set(UNITY_SOURCE ${LIBS_DIR}/unity/unity.c)
# base binary directory for tests compilation output
set(TESTS_BIN_DIR ${CMAKE_BINARY_DIR}/mytests) 
# byte code version. Format: [Y][M][D][00-99 (patch id)]
set(BYTECODE_VERSION 2026060400)

# ---------- FUNCTIONS ----------

# add every top level module in MODULES_DIR as separate shared library
function(add_flow_modules)
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
            target_include_directories(${module_name} PUBLIC ${module_path})
        endif()
    endforeach()
endfunction()

# make an executable from every test file in TESTS_DIR directory
function(add_tests)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${TESTS_BIN_DIR})
    file(GLOB_RECURSE test_files CONFIGURE_DEPENDS ${TESTS_DIR}/*.test.c)
    foreach(test_file ${test_files})
        cmake_path(GET test_file FILENAME test_name)
        add_executable(${test_name} ${test_file} ${UNITY_SOURCE})
        add_test(NAME ${test_name} COMMAND ${test_name})
    endforeach()
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
endfunction()

# ---------- TARGETS ----------

add_library(flow STATIC)
link_libraries(flow)
add_executable(main)
add_flow_modules()

if (BUILD_TESTING)
    add_tests()
endif()

# ---------- CONFIG ----------

include_directories(${CORE_DIR} ${LIBS_DIR})

add_compile_definitions(
    _INCLUDE_PROFILER=0
    _INCLUDE_INLINE_LOGS=0
    _SUPPRESS_ERRORS=0
    _C_COMPILER_PATH="${CMAKE_C_COMPILER}"
    _TEST_INPUT_PATH="${SAMPLE_PATH}"
    _PROFILER_LOGS_PATH="${LOG_DIR}/profiler.txt"
    _BYTECODE_VERSION="${BYTECODE_VERSION}"
)

# ---------- SOURCES ----------

# gather main source files
file(GLOB_RECURSE flow_sources CONFIGURE_DEPENDS ${CORE_DIR}/*.c)
file(GLOB_RECURSE tests_sources CONFIGURE_DEPENDS ${TESTS_DIR}/*.test.c)
file(GLOB_RECURSE unity_sources CONFIGURE_DEPENDS ${LIBS_DIR}/unity/*.c)

# add sources
target_sources(flow PRIVATE ${flow_sources})
target_sources(main PRIVATE ${CMAKE_SOURCE_DIR}/main.c)