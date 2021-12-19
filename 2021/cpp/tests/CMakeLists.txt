cmake_minimum_required(VERSION 3.16)

if(NOT DEFINED AOC_Y2021_DAYS_LIST)
message(FATAL_ERROR variable missing)
endif()

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

include(GoogleTest)

function(AOC_Y2021_CreateTest TEST_DAY)
    set(AOC_Y2021_TEST_TARGET_NAME AOC_Y2021_test_day${TEST_DAY})
    add_executable(${AOC_Y2021_TEST_TARGET_NAME}
        ${CMAKE_CURRENT_SOURCE_DIR}/test_day${TEST_DAY}.cpp
        ${AOC_Y2021_SOURCE_DIR}/day${TEST_DAY}.cpp
    )

    get_target_property(AOC_Y2021_INCLUDES AOC_Y2021 INCLUDE_DIRECTORIES)
    target_include_directories(${AOC_Y2021_TEST_TARGET_NAME} PRIVATE
        ${AOC_Y2021_SOURCE_DIR}
        ${AOC_Y2021_INCLUDES}
    )

    default_compile_options(${AOC_Y2021_TEST_TARGET_NAME})

    target_link_libraries(${AOC_Y2021_TEST_TARGET_NAME}
        gtest_main
    )

    gtest_discover_tests(${AOC_Y2021_TEST_TARGET_NAME})
endfunction(AOC_Y2021_CreateTest)

foreach(DAY ${AOC_Y2021_DAYS_LIST})
    AOC_Y2021_CreateTest(${DAY})
endforeach()