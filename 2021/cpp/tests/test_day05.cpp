
#include "ipuzzle.hpp"
#include "puzzle_common.hpp"

#include <gtest/gtest.h>

#include <memory>
#include <string_view>

namespace AOC::Y2021
{

AOC_Y2021_PUZZLE_CLASS_DECLARATION(05)

namespace
{

class Day05Test : public ::testing::Test
{
  protected:
    void SetUp() override
    {
        pPuzzle = std::make_unique<PuzzleDay05>(exampleInput);
    }

    // void TearDown() override {}
    std::unique_ptr<IPuzzle> pPuzzle;
    static constexpr std::string_view exampleInput =
        "0,9 -> 5,9\r\n8,0 -> 0,8\n9,4 -> 3,4\n2,2 -> 2,1\r\n7,0 -> 7,4\r\n6,4 -> 2,0\n0,9 "
        "-> 2,9\r\n3,4 -> 1,4\r\n0,0 -> 8,8\r\n5,5 -> 8,2";
};

TEST_F(Day05Test, Part1)
{
    ASSERT_EQ(std::get<std::int64_t>(pPuzzle->Part1()), 5);
}

TEST_F(Day05Test, Part2)
{
    ASSERT_EQ(std::get<std::int64_t>(pPuzzle->Part2()), 12);
}

} // namespace
} // namespace AOC::Y2021
