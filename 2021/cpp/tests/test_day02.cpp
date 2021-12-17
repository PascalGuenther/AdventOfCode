
#include "ipuzzle.hpp"
#include "puzzle_common.hpp"

#include <gtest/gtest.h>

#include <memory>
#include <string_view>

namespace AOC::Y2021
{

AOC_Y2021_PUZZLE_CLASS_DECLARATION(02)

namespace
{

class Day02Test : public ::testing::Test
{
  protected:
    void SetUp() override
    {
        pPuzzle = std::make_unique<PuzzleDay02>(exampleInput);
    }

    // void TearDown() override {}
    std::unique_ptr<IPuzzle> pPuzzle;
    static constexpr std::string_view exampleInput = "forward 5\r\ndown 5\nforward 8\nup 3\ndown 8\nforward 2\r";
};

TEST_F(Day02Test, Part1)
{
    ASSERT_EQ(std::get<std::int64_t>(pPuzzle->Part1()), 150);
}

TEST_F(Day02Test, Part2)
{
    ASSERT_EQ(std::get<std::int64_t>(pPuzzle->Part2()), 900);
}

} // namespace
} // namespace AOC::Y2021
