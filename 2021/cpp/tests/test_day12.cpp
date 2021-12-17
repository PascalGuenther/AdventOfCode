
#include "ipuzzle.hpp"
#include "puzzle_common.hpp"

#include <gtest/gtest.h>

#include <cstdint>
#include <memory>
#include <string_view>

namespace AOC::Y2021
{

AOC_Y2021_PUZZLE_CLASS_DECLARATION(12)

namespace
{

struct Day12TestParams final
{
    std::string_view input;
    std::uint64_t expectedResultPart1;
    std::uint64_t expectedResultPart2;
};

class Day12Test : public testing::TestWithParam<Day12TestParams>
{
  protected:
    void SetUp() override
    {
        pPuzzle = std::make_unique<PuzzleDay12>(GetParam().input);
    }

    // void TearDown() override {}
    std::unique_ptr<IPuzzle> pPuzzle;
};

TEST_P(Day12Test, Part1)
{
    ASSERT_EQ(pPuzzle->Part1(), GetParam().expectedResultPart1);
}

TEST_P(Day12Test, Part2)
{
    const auto res = pPuzzle->Part2();
    ASSERT_TRUE(res.has_value());
    ASSERT_EQ(res.value(), GetParam().expectedResultPart2);
}

constexpr Day12TestParams day12ExamplesInOut[] = {
    {
        .input{"start-A\nstart-b\nA-c\nA-b\nb-d\nA-end\r\nb-end"},
        .expectedResultPart1 = 10,
        .expectedResultPart2 = 36,
    },
    {

        .input{"dc-end\r\nHN-start\r\nstart-kj\r\ndc-start\r\ndc-HN\r\nLN-dc\r\nHN-end\r\nkj-sa\r\nkj-HN\r\nkj-dc\r"},
        .expectedResultPart1 = 19,
        .expectedResultPart2 = 103,
    },
    {

        .input{"fs-end\nhe-DX\nfs-he\nstart-DX\npj-DX\nend-zg\nzg-sl\nzg-pj\npj-he\nRW-he\nfs-DX\npj-RW\nzg-RW\nstart-"
               "pj\nhe-WI\nzg-he\npj-fs\nstart-RW\n"},
        .expectedResultPart1 = 226,
        .expectedResultPart2 = 3509,
    },
};
INSTANTIATE_TEST_SUITE_P(Day12Examples, Day12Test, testing::ValuesIn(day12ExamplesInOut));

} // namespace
} // namespace AOC::Y2021
