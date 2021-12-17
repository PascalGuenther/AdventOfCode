
#include "ipuzzle.hpp"
#include "puzzle_common.hpp"

#include <gtest/gtest.h>

#include <memory>
#include <string_view>

namespace AOC::Y2021
{

AOC_Y2021_PUZZLE_CLASS_DECLARATION(10)

namespace
{

class Day10Test : public ::testing::Test
{
  protected:
    void SetUp() override
    {
        pPuzzle = std::make_unique<PuzzleDay10>(exampleInput);
    }

    // void TearDown() override {}
    std::unique_ptr<IPuzzle> pPuzzle;
    static constexpr std::string_view exampleInput =
        "[({(<(())[]>[[{[]{<()<>>\r\n[(()[<>])]({[<{<<[]>>(\r\n{([(<{}[<>[]}>{[]{[(<()>\n(((({<>}<{<{<>}{[]{[]{}\r\n["
        "[<[([]))<([[{}[[()]]]\r\n[{[{({}]{}}([{[{{{}}([]\r\n{<[[]]>}<{[{[{[]{()[[[]\r\n[<(<(<(<{}))><([]([]()\r\n<{([("
        "[[(<>()){}]>(<<{{\r\n<{([{{}}[<[[[<>{}]]]>[]]";
};

TEST_F(Day10Test, Part1)
{
    ASSERT_EQ(std::get<std::int64_t>(pPuzzle->Part1()), 26'397);
}

TEST_F(Day10Test, Part2)
{
    ASSERT_EQ(std::get<std::int64_t>(pPuzzle->Part2()), 288'957);
}

} // namespace
} // namespace AOC::Y2021
