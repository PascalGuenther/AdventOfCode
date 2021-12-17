
#include "ipuzzle.hpp"
#include "puzzle_common.hpp"

#include <gtest/gtest.h>

#include <memory>
#include <string>
#include <string_view>

namespace AOC::Y2021
{

AOC_Y2021_PUZZLE_CLASS_DECLARATION(13)

namespace
{

class Day13Test : public ::testing::Test
{
  protected:
    void SetUp() override
    {
        pPuzzle = std::make_unique<PuzzleDay13>(exampleInput);
    }

    // void TearDown() override {}
    std::unique_ptr<IPuzzle> pPuzzle;

    static constexpr std::string_view exampleInput =
        "6,10\r\n0,14\r\n9,10\r\n0,3\r\n10,4\r\n4,11\r\n6,0\r\n6,12\n4,1\r\n0,13\r\n10,12\r\n3,4\r\n3,0\r\n8,4\r\n1,"
        "10\r\n2,14\r\n8,10\r\n9,0\r\n\r\nfold along y=7\nfold along x=5";
};

TEST_F(Day13Test, Part1)
{
    ASSERT_EQ(std::get<std::int64_t>(pPuzzle->Part1()), 17);
}

TEST_F(Day13Test, Part2)
{
    constexpr auto expectedFoldingPattern = "#####\n#...#\n#...#\n#...#\n#####\n";
    ASSERT_EQ(std::get<std::string>(pPuzzle->Part2()), expectedFoldingPattern);
}

} // namespace
} // namespace AOC::Y2021
