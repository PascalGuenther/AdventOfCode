
#include "ipuzzle.hpp"
#include "puzzle_common.hpp"

#include <gtest/gtest.h>

#include <memory>
#include <string_view>

namespace AOC::Y2021
{

AOC_Y2021_PUZZLE_CLASS_DECLARATION(11)

namespace
{

class Day11Test : public ::testing::Test
{
  protected:
    void SetUp() override
    {
        pPuzzle = std::make_unique<PuzzleDay11>(exampleInput);
    }

    // void TearDown() override {}
    std::unique_ptr<IPuzzle> pPuzzle;
    static constexpr std::string_view exampleInput = "5483143223\n2745854711\n5264556173\n6141336146\n6357385478\n41675"
                                                     "24645\n2176841721\n6882881134\n4846848554\n5283751526\n";
};

TEST_F(Day11Test, Part1)
{
    ASSERT_EQ(std::get<std::int64_t>(pPuzzle->Part1()), 1656);
}

TEST_F(Day11Test, Part2)
{
    ASSERT_EQ(std::get<std::int64_t>(pPuzzle->Part2()), 195);
}

} // namespace
} // namespace AOC::Y2021
