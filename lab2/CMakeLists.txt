cmake_minimum_required(VERSION 3.16)

project(Dekanat
    VERSION 1.0
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
)

add_library(dekanat STATIC ${SOURCES})

target_include_directories(dekanat PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

target_compile_options(dekanat PRIVATE
    -Wall
    -Wextra
    -Wpedantic
)

# Основное приложение
add_executable(dekanat_app main.cpp)

target_link_libraries(dekanat_app PRIVATE dekanat)

target_compile_options(dekanat_app PRIVATE
    -Wall
    -Wextra
    -Wpedantic
)
