
#include "ipuzzle.hpp"
#include "puzzle_common.hpp"

#include <gtest/gtest.h>

#include <cstdint>
#include <memory>
#include <string_view>

namespace AOC::Y2021
{

AOC_Y2021_PUZZLE_CLASS_DECLARATION(08)

namespace
{

struct Day08TestParams final
{
    std::string_view input;
    std::uint64_t expectedResultPart1;
    std::uint64_t expectedResultPart2;
};

class Day08Test : public testing::TestWithParam<Day08TestParams>
{
  protected:
    void SetUp() override
    {
        pPuzzle = std::make_unique<PuzzleDay08>(GetParam().input);
    }

    // void TearDown() override {}
    std::unique_ptr<IPuzzle> pPuzzle;
};

TEST_P(Day08Test, Part1)
{
    ASSERT_EQ(pPuzzle->Part1(), GetParam().expectedResultPart1);
}

TEST_P(Day08Test, Part2)
{
    ASSERT_EQ(pPuzzle->Part2(), GetParam().expectedResultPart2);
}

constexpr Day08TestParams day08ExamplesInOut[] = {
    {
        .input{"acedgfb cdfbe gcdfa fbcad dab cefabd cdfgeb eafb cagedb ab | cdfeb fcadb cdfeb cdbaf"},
        .expectedResultPart1{0},
        .expectedResultPart2{5353},
    },
    {
        .input{
            "be cfbegad cbdgef fgaecd cgeb fdcge agebfd fecdb fabcd edb | fdgacbe cefdb cefbgd gcbe\nedbfga begcd cbg "
            "gc "
            "gcadebf fbgde acbgfd abcde gfcbed gfec | fcgedb cgb dgebacf gc\nfgaebd cg bdaec gdafb agbcfd gdcbef bgcad "
            "gfac gcb cdgabef | cg cg fdcagb cbg\nfbegcd cbd adcefb dageb afcb bc aefdc ecdab fgdeca fcdbega | efabcd "
            "cedba gadfec cb\naecbfdg fbg gf bafeg dbefa fcge gcbea fcaegb dgceab fcbdga | gecf egdcabf bgf "
            "bfgea\nfgeab "
            "ca afcebg bdacfeg cfaedg gcfdb baec bfadeg bafgc acf | gebdcfa ecba ca fadegcb\ndbcfg fgd bdegcaf fgec "
            "aegbdf "
            "ecdfab fbedc dacgb gdcebf gf | cefg dcbef fcge gbcadfe\nbdfegc cbegaf gecbf dfcage bdacg ed bedf ced "
            "adcbefg "
            "gebcd | ed bcgafe cdgba cbgef\negadfb cdbfeg cegd fecab cgb gbdefca cg fgcdab egfdb bfceg | gbdfcae bgc "
            "cg "
            "cgb\ngcafb gcf dcaebfg ecagb gf abcdeg gaef cafbge fdbac fegbdc | fgae cfgab fg bagce"},
        .expectedResultPart1{26},
        .expectedResultPart2{61229},
    },
};
INSTANTIATE_TEST_SUITE_P(Day08Examples, Day08Test, testing::ValuesIn(day08ExamplesInOut));

} // namespace
} // namespace AOC::Y2021
