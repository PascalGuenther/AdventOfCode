cmake_minimum_required(VERSION 3.16)

add_executable(AOC_Y2021_CLI)

target_link_libraries(AOC_Y2021_CLI PRIVATE
    AOC_Y2021
)

add_subdirectory(src)

default_compile_options(AOC_Y2021_CLI)
