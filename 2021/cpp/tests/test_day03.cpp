
#include "ipuzzle.hpp"
#include "puzzle_common.hpp"

#include <gtest/gtest.h>

#include <memory>
#include <string_view>

namespace AOC::Y2021
{

AOC_Y2021_PUZZLE_CLASS_DECLARATION(03)

namespace
{

class Day03Test : public ::testing::Test
{
  protected:
    void SetUp() override
    {
        pPuzzle = std::make_unique<PuzzleDay03>(exampleInput);
    }

    // void TearDown() override {}
    std::unique_ptr<IPuzzle> pPuzzle;
    static constexpr std::string_view exampleInput =
        "00100\n11110\r\n10110\r\n10111\r\n10101\r\n01111\r\n00111\r\n11100\n10000\r\n11001\r\n00010\r\n01010\r";
};

TEST_F(Day03Test, Part1)
{
    ASSERT_EQ(pPuzzle->Part1(), 198);
}

TEST_F(Day03Test, Part2)
{
    ASSERT_EQ(pPuzzle->Part2(), 230);
}

} // namespace
} // namespace AOC::Y2021
