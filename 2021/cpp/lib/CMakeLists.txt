cmake_minimum_required(VERSION 3.16)

add_library(AOC_Y2021 STATIC
    src/day01.cpp
    src/day02.cpp
    src/puzzle_factory.cpp
    src/utils.cpp
)

target_include_directories(AOC_Y2021 PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/inc
)

option(AOC_Y2021_CONSTEXPR_UNIT_TEST "Execute unit tests at compile time (requires C++20)" OFF)

if (AOC_Y2021_CONSTEXPR_UNIT_TEST)
    target_compile_definitions(AOC_Y2021 PRIVATE
        AOC_Y2021_CONSTEXPR_UNIT_TEST=1
    )
endif()