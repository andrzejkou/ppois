cmake_minimum_required(VERSION 3.16)
project(WarehouseManagement VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

file(GLOB_RECURSE WAREHOUSE_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/cpp/*.cpp"
)

add_library(warehouse ${WAREHOUSE_SOURCES})
target_include_directories(warehouse PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/h"
    "${CMAKE_CURRENT_SOURCE_DIR}/h/people"
    "${CMAKE_CURRENT_SOURCE_DIR}/h/products"
    "${CMAKE_CURRENT_SOURCE_DIR}/h/storage"
    "${CMAKE_CURRENT_SOURCE_DIR}/h/inventory"
    "${CMAKE_CURRENT_SOURCE_DIR}/h/procurement"
    "${CMAKE_CURRENT_SOURCE_DIR}/h/orders"
    "${CMAKE_CURRENT_SOURCE_DIR}/h/logistics"
    "${CMAKE_CURRENT_SOURCE_DIR}/h/finance"
    "${CMAKE_CURRENT_SOURCE_DIR}/h/management"
    "${CMAKE_CURRENT_SOURCE_DIR}/h/exceptions"
)
target_compile_options(warehouse PRIVATE -Wall -Wextra -Wpedantic)

add_executable(warehouse_app main.cpp)
target_link_libraries(warehouse_app PRIVATE warehouse)

include(CTest)
option(ENABLE_COVERAGE "Build with GCC coverage instrumentation" OFF)
if(ENABLE_COVERAGE)
    if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
        target_compile_options(warehouse PRIVATE --coverage -O0 -g)
        target_link_libraries(warehouse PUBLIC gcov)
        add_compile_options(--coverage -O0 -g)
        add_link_options(--coverage)
    else()
        message(FATAL_ERROR "ENABLE_COVERAGE currently supports GCC only")
    endif()
endif()

if(BUILD_TESTING)
    add_executable(warehouse_tests tests/test_warehouse.cpp)
    target_link_libraries(warehouse_tests PRIVATE warehouse)
    add_test(NAME warehouse_smoke_tests COMMAND warehouse_tests)

    add_executable(warehouse_generated_tests tests/test_generated_entities.cpp)
    target_link_libraries(warehouse_generated_tests PRIVATE warehouse)
    add_test(NAME warehouse_generated_entities COMMAND warehouse_generated_tests)

    add_executable(warehouse_edge_tests tests/test_coverage_edges.cpp)
    target_link_libraries(warehouse_edge_tests PRIVATE warehouse)
    add_test(NAME warehouse_edge_cases COMMAND warehouse_edge_tests)

    add_executable(warehouse_per_file_tests tests/test_per_file_coverage.cpp)
    target_link_libraries(warehouse_per_file_tests PRIVATE warehouse)
    add_test(NAME warehouse_per_file_coverage COMMAND warehouse_per_file_tests)
endif()

if(ENABLE_COVERAGE)
    find_package(Python3 COMPONENTS Interpreter REQUIRED)
    add_custom_target(coverage
        COMMAND ${Python3_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/tools/coverage.py
            --build-dir ${CMAKE_BINARY_DIR}
            --source-dir ${CMAKE_CURRENT_SOURCE_DIR}
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        DEPENDS warehouse_tests warehouse_generated_tests warehouse_edge_tests warehouse_per_file_tests
        COMMENT "Running tests and calculating GCC line/branch coverage"
        VERBATIM
    )
endif()
