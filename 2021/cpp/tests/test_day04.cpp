
#include "ipuzzle.hpp"
#include "puzzle_common.hpp"

#include <gtest/gtest.h>

#include <memory>
#include <string_view>

namespace AOC::Y2021
{

AOC_Y2021_PUZZLE_CLASS_DECLARATION(04)

namespace
{

class Day04Test : public ::testing::Test
{
  protected:
    void SetUp() override
    {
        pPuzzle = std::make_unique<PuzzleDay04>(exampleInput);
    }

    // void TearDown() override {}
    std::unique_ptr<IPuzzle> pPuzzle;
    static constexpr std::string_view exampleInput =
        "7,4,9,5,11,17,23,2,0,14,21,24,10,16,13,6,15,25,12,22,18,20,8,19,3,26,1\n\r\n22 13 17 11  0\r\n 8  2 23  4 "
        "24\r\n21  9 14 16  7\r\n 6 10  3 18  5\r\n 1 12 20 15 19\n\r\n 3 15  0  2 22\r\n 9 18 13 17  5\n19  8  7 25 "
        "23\r\n20 11 10 24  4\n14 21 16 12  6\r\n\n14 21 17 24  4\r\n10 16 15  9 19\r\n18  8 23 26 20\r\n22 11 13  6  "
        "5\r\n 2  0 12  3  7\r";
};

TEST_F(Day04Test, Part1)
{
    ASSERT_EQ(pPuzzle->Part1(), 4512);
}

TEST_F(Day04Test, Part2)
{
    ASSERT_EQ(pPuzzle->Part2(), 1924);
}

} // namespace
} // namespace AOC::Y2021
