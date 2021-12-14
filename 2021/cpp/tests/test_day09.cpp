
#include "ipuzzle.hpp"
#include "puzzle_common.hpp"

#include <gtest/gtest.h>

#include <memory>
#include <string_view>

namespace AOC::Y2021
{

AOC_Y2021_PUZZLE_CLASS_DECLARATION(09)

namespace
{

class Day09Test : public ::testing::Test
{
  protected:
    void SetUp() override
    {
        pPuzzle = std::make_unique<PuzzleDay09>(exampleInput);
    }

    // void TearDown() override {}
    std::unique_ptr<IPuzzle> pPuzzle;
    static constexpr std::string_view exampleInput = "2199943210\n3987894921\n9856789892\n8767896789\n9899965678";
};

TEST_F(Day09Test, Part1)
{
    ASSERT_EQ(pPuzzle->Part1(), 15);
}

TEST_F(Day09Test, Part2)
{
    ASSERT_EQ(pPuzzle->Part2(), 1134);
}

} // namespace
} // namespace AOC::Y2021
