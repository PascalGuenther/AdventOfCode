cmake_minimum_required(VERSION 3.16)

project(AdventOfCode VERSION 2021.12.9)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

function(default_compile_options target)
    if(MSVC)
        target_compile_options(${target} PRIVATE
            /W4
            /WX
        )
    else()
        target_compile_options(${target} PRIVATE
            -Wall
            -Wpedantic
            -Wextra
            -Werror
            -O3
        )
    endif()
endfunction()

add_subdirectory(lib)

add_executable(AOC_Y2021_CLI
    ${CMAKE_CURRENT_SOURCE_DIR}/cli.cpp
)

target_link_libraries(AOC_Y2021_CLI PRIVATE
    AOC_Y2021
)

default_compile_options(AOC_Y2021_CLI)
default_compile_options(AOC_Y2021)
