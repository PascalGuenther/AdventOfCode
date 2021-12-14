
#include "ipuzzle.hpp"
#include "puzzle_common.hpp"

#include <gtest/gtest.h>

#include <memory>
#include <string_view>

namespace AOC::Y2021
{

AOC_Y2021_PUZZLE_CLASS_DECLARATION(06)

namespace
{

class Day06Test : public ::testing::Test
{
  protected:
    void SetUp() override
    {
        pPuzzle = std::make_unique<PuzzleDay06>(exampleInput);
    }

    // void TearDown() override {}
    std::unique_ptr<IPuzzle> pPuzzle;
    static constexpr std::string_view exampleInput = "3,4,3,1,2";
};

TEST_F(Day06Test, Part1)
{
    ASSERT_EQ(pPuzzle->Part1(), 5934);
}

TEST_F(Day06Test, Part2)
{
    ASSERT_EQ(pPuzzle->Part2(), 26984457539);
}

} // namespace
} // namespace AOC::Y2021
