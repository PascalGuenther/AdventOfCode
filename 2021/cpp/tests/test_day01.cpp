
#include "ipuzzle.hpp"
#include "puzzle_common.hpp"

#include <gtest/gtest.h>

#include <memory>
#include <string_view>

namespace AOC::Y2021
{

AOC_Y2021_PUZZLE_CLASS_DECLARATION(01)

namespace
{

class Day01Test : public ::testing::Test
{
  protected:
    void SetUp() override
    {
        pPuzzle = std::make_unique<PuzzleDay01>(exampleInput);
    }

    // void TearDown() override {}
    std::unique_ptr<IPuzzle> pPuzzle;
    static constexpr std::string_view exampleInput = "199\r\n200\n208\n210\n200\n207\n240\n269\n260\n263";
};

TEST_F(Day01Test, Part1)
{
    ASSERT_EQ(pPuzzle->Part1(), 7);
}

TEST_F(Day01Test, Part2)
{
    ASSERT_EQ(pPuzzle->Part2(), 5);
}

} // namespace
} // namespace AOC::Y2021
