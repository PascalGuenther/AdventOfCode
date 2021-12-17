cmake_minimum_required(VERSION 3.16)

project(AdventOfCode VERSION 2021.12.12)

if(MSVC)
    set(CMAKE_CXX_STANDARD 23)
else()
    set(CMAKE_CXX_STANDARD 20)
endif()
set(CMAKE_CXX_STANDARD_REQUIRED True)

function(default_compile_options target)
    if(MSVC)
        target_compile_options(${target} PRIVATE
            /W4
        )
    else()
        target_compile_options(${target} PRIVATE
            -Wall
            -Wpedantic
            -Wextra
            -Werror
        )
    endif()
endfunction()

add_library(AOC_Y2021 STATIC)

option(AOC_Y2021_CONSTEXPR_UNIT_TEST "Execute unit tests at compile time (requires C++20)" OFF)
if (AOC_Y2021_CONSTEXPR_UNIT_TEST)
    target_compile_definitions(AOC_Y2021 PRIVATE
        AOC_Y2021_CONSTEXPR_UNIT_TEST=1
    )
endif()

default_compile_options(AOC_Y2021)

add_subdirectory(include)
add_subdirectory(src)

add_subdirectory(examples)

enable_testing()
add_subdirectory(tests)
