cmake_minimum_required(VERSION 3.16)


set(AOC_Y2021_DAYS_LIST "01;02;03;04;05;06;07;08;09;10;11;12;13;14;15")
set(AOC_Y2021_DAYS_LIST ${AOC_Y2021_DAYS_LIST} PARENT_SCOPE)
set(AOC_Y2021_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR} PARENT_SCOPE)


target_sources(AOC_Y2021  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/puzzle_factory.cpp
)

foreach(DAY ${AOC_Y2021_DAYS_LIST})
    target_sources(AOC_Y2021 PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/day${DAY}.cpp
    )
endforeach()
