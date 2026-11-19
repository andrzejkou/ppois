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
if(BUILD_TESTING)
    add_executable(warehouse_tests tests/test_warehouse.cpp)
    target_link_libraries(warehouse_tests PRIVATE warehouse)
    add_test(NAME warehouse_smoke_tests COMMAND warehouse_tests)
endif()
