
cmake_minimum_required(VERSION 3.16)

project(Dekanat VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

option(ENABLE_COVERAGE "Build with GCC code coverage instrumentation" OFF)

# Исходники предметной области
file(GLOB_RECURSE DEKANAT_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
)

# Библиотека деканата
add_library(dekanat STATIC ${DEKANAT_SOURCES})

target_include_directories(dekanat PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

target_compile_options(dekanat PRIVATE
    -Wall -Wextra -Wpedantic
)

# Проверка приложения
add_executable(dekanat_app main.cpp)

target_link_libraries(dekanat_app PRIVATE dekanat)

target_compile_options(dekanat_app PRIVATE
    -Wall -Wextra -Wpedantic
)

# Подключение тестирования
include(CTest)

if(BUILD_TESTING)
    find_package(GTest REQUIRED)

    file(GLOB TEST_SOURCES CONFIGURE_DEPENDS
        "${CMAKE_CURRENT_SOURCE_DIR}/tests/*.cpp"
    )

    add_executable(dekanat_tests ${TEST_SOURCES})

    target_link_libraries(dekanat_tests PRIVATE
        dekanat
        GTest::gtest_main
    )

    target_compile_options(dekanat_tests PRIVATE
        -Wall -Wextra -Wpedantic
    )

    include(GoogleTest)
    gtest_discover_tests(dekanat_tests)
endif()

# Настройка покрытия
if(ENABLE_COVERAGE)
    if(NOT CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
        message(FATAL_ERROR "ENABLE_COVERAGE currently supports GCC only")
    endif()

    if(NOT BUILD_TESTING)
        message(FATAL_ERROR "Coverage requires BUILD_TESTING=ON")
    endif()

    # Инструментирование предметной библиотеки
    target_compile_options(dekanat PRIVATE
        --coverage
        -O0
        -g
    )

    target_link_options(dekanat INTERFACE --coverage)

    # Поиск gcovr
    find_program(GCOVR_EXECUTABLE gcovr)

    if(NOT GCOVR_EXECUTABLE)
        message(FATAL_ERROR "gcovr not found. Install it before enabling coverage.")
    endif()

    # HTML и XML отчёты.
    # Перед измерением автоматически запускаются тесты.
    add_custom_target(coverage_report
        COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure

        COMMAND ${GCOVR_EXECUTABLE}
            --root "${CMAKE_SOURCE_DIR}"
            --filter "${CMAKE_SOURCE_DIR}/src/"
            --html-details
            --output "${CMAKE_BINARY_DIR}/coverage.html"
            "${CMAKE_BINARY_DIR}"

        COMMAND ${GCOVR_EXECUTABLE}
            --root "${CMAKE_SOURCE_DIR}"
            --filter "${CMAKE_SOURCE_DIR}/src/"
            --xml-pretty
            --output "${CMAKE_BINARY_DIR}/coverage.xml"
            "${CMAKE_BINARY_DIR}"

        WORKING_DIRECTORY "${CMAKE_BINARY_DIR}"
        COMMENT "Running tests and generating coverage reports"
        VERBATIM
    )

    # Проверка достижения покрытия 90%
    add_custom_target(coverage_check
        COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure

        COMMAND ${GCOVR_EXECUTABLE}
            --root "${CMAKE_SOURCE_DIR}"
            --filter "${CMAKE_SOURCE_DIR}/src/"
            --fail-under-line 90
            --fail-under-function 90
            --print-summary
            "${CMAKE_BINARY_DIR}"

        WORKING_DIRECTORY "${CMAKE_BINARY_DIR}"
        COMMENT "Checking code coverage threshold: 90%"
        VERBATIM
    )
endif()

