
#include "ipuzzle.hpp"
#include "puzzle_common.hpp"

#include <gtest/gtest.h>

#include <memory>
#include <string_view>

namespace AOC::Y2021
{

AOC_Y2021_PUZZLE_CLASS_DECLARATION(07)

namespace
{

class Day07Test : public ::testing::Test
{
  protected:
    void SetUp() override
    {
        pPuzzle = std::make_unique<PuzzleDay07>(exampleInput);
    }

    // void TearDown() override {}
    std::unique_ptr<IPuzzle> pPuzzle;
    static constexpr std::string_view exampleInput = "16,1,2,0,4,2,7,1,2,14\r";
};

TEST_F(Day07Test, Part1)
{
    ASSERT_EQ(pPuzzle->Part1(), 37);
}

TEST_F(Day07Test, Part2)
{
    ASSERT_EQ(pPuzzle->Part2(), 168);
}

} // namespace
} // namespace AOC::Y2021
