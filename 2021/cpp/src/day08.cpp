#include "ipuzzle.hpp"
#include "puzzle_common.hpp"
#include "utils.hpp"

#include <algorithm>
#include <array>
#include <bit>
#include <cstddef>
#include <cstdint>
#include <memory>
#include <numeric>
#include <string_view>
#include <utility>
#include <vector>

namespace AOC::Y2021
{

namespace Day08
{

namespace
{

using SegmentRepresentation = std::uint8_t;

using DisplayPatternAndValue = std::pair<std::array<SegmentRepresentation, 10U>, std::array<SegmentRepresentation, 4U>>;

constexpr size_t leastNumOfSegments{2};
constexpr char segMin{'a'};
constexpr char segMax{'g'};
static_assert((segMax - segMin) + 1, "Day 08: invalid number of segments for a 7 segment display");

constexpr SegmentRepresentation ParseSegmentRepresentation(const std::string_view &pattern)
{
    SegmentRepresentation ret{0};
    if (pattern.size() < leastNumOfSegments)
    {
        return 0;
    }
    for (const auto &segment : pattern)
    {
        if ((segment < segMin) || (segment > segMax))
        {
            return 0;
        }
        ret = (ret | (1U << (segment - segMin))) & 0xFF;
    }
    return ret;
}

constexpr std::array<SegmentRepresentation, 10U> correctRepresentations = {
    /* 0 */ ParseSegmentRepresentation("abcefg"),
    /* 1 */ ParseSegmentRepresentation("cf"),
    /* 2 */ ParseSegmentRepresentation("acdeg"),
    /* 3 */ ParseSegmentRepresentation("acdfg"),
    /* 4 */ ParseSegmentRepresentation("bcdf"),
    /* 5 */ ParseSegmentRepresentation("abdfg"),
    /* 6 */ ParseSegmentRepresentation("abdefg"),
    /* 7 */ ParseSegmentRepresentation("acf"),
    /* 8 */ ParseSegmentRepresentation("abcdefg"),
    /* 9 */ ParseSegmentRepresentation("abcdfg"),
};

AOC_Y2021_CONSTEXPR std::vector<DisplayPatternAndValue> ParseInput(std::string_view input)
{
    std::vector<DisplayPatternAndValue> ret;
    while (input.size() > 5)
    {
        const auto endOfLine = input.find('\n');
        const bool bIsLastLine = endOfLine == input.npos;
        const auto line = (bIsLastLine) ? input : input.substr(0, endOfLine);
        const auto delimiterPos = line.find('|');
        if (delimiterPos == input.npos)
        {
            return {};
        }
        const auto signalPatterns = line.substr(0, delimiterPos);
        constexpr std::string_view segmentsLetters = "abcdefg";
        const auto outputDigits = line.substr(delimiterPos + 1);
        if ((signalPatterns.size() < 5) || (outputDigits.size() < 5) ||
            (signalPatterns.find_first_of(segmentsLetters) != 0) || (outputDigits.find_first_of(segmentsLetters) != 1))
        {
            return {};
        }

        auto fnSegmentParser = [&segmentsLetters](std::string_view in, std::size_t outSize,
                                                  SegmentRepresentation &out) {
            for (size_t i = 0; i != outSize; ++i)
            {
                const auto patternStart = in.find_first_of(segmentsLetters);

                if (patternStart == in.npos)
                {
                    return false;
                }
                const auto patternEnd = in.find_first_not_of(segmentsLetters, patternStart);
                if (patternEnd == in.npos)
                {
                    const bool bError = (i != (outSize - 1));
                    if (bError)
                    {
                        return false;
                    }
                }
                const auto pattern = (patternEnd == in.npos) ? in.substr(patternStart)
                                                             : in.substr(patternStart, patternEnd - patternStart);
                auto parsed = ParseSegmentRepresentation(pattern);
                if (parsed == 0)
                {
                    return false;
                }
                (&out)[i] = parsed;

                in.remove_prefix(patternEnd + 1);
            }
            return true;
        };
        auto &elem = ret.emplace_back();
        if (!fnSegmentParser(signalPatterns, elem.first.size(), *elem.first.data()) ||
            !fnSegmentParser(outputDigits, elem.second.size(), *elem.second.data()))
        {
            return {};
        }

        if (bIsLastLine)
        {
            break;
        }
        else
        {
            input.remove_prefix(endOfLine + 1);
        }
    }
    return ret;
}

AOC_Y2021_CONSTEXPR auto CountEasilyTdentifiableDigits(const auto &displays)
{
    return std::accumulate(std::begin(displays), std::end(displays), 0, [](const auto &acc, const auto &display) {
        const auto cnt = std::count_if(std::begin(display.second), std::end(display.second), [](const auto &num) {
            const auto numEnabledSegments = std::popcount(num);
            switch (numEnabledSegments)
            {
            case std::popcount(correctRepresentations[1]):
            case std::popcount(correctRepresentations[4]):
            case std::popcount(correctRepresentations[7]):
            case std::popcount(correctRepresentations[8]):
                return true;
            default:
                return false;
            }
        });
        return acc + cnt;
    });
}

AOC_Y2021_CONSTEXPR auto MapPatterns(const std::array<SegmentRepresentation, 10u> &aScrambledPatterns)
{
    std::array<SegmentRepresentation, 10u> aRet{};
#define AOC_Y2021_Day08_TRY_EXTRACT_EASY(number)                                                                       \
    const auto it##number = std::find_if(aScrambledPatterns.begin(), aScrambledPatterns.end(), [](const auto &rep) {   \
        return (std::popcount(rep) == std::popcount(correctRepresentations[number]));                                  \
    });                                                                                                                \
    if (it##number == aScrambledPatterns.end())                                                                        \
    {                                                                                                                  \
        return aRet;                                                                                                   \
    }                                                                                                                  \
    aRet[number] = *it##number;

    AOC_Y2021_Day08_TRY_EXTRACT_EASY(1) AOC_Y2021_Day08_TRY_EXTRACT_EASY(4) AOC_Y2021_Day08_TRY_EXTRACT_EASY(7)
        AOC_Y2021_Day08_TRY_EXTRACT_EASY(8)
#undef AOC_Y2021_Day08_TRY_EXTRACT_EASY

#define AOC_Y2021_Day08_STORE_IT_OR_RETURN(num)                                                                        \
    if (it##num == aScrambledPatterns.end())                                                                           \
    {                                                                                                                  \
        return aRet;                                                                                                   \
    }                                                                                                                  \
    aRet[num] = *it##num;

            constexpr auto segmentMask = correctRepresentations[8];

    // const auto segA = (aRet[1] ^ aRet[7]) & segmentMask;

    const auto it6 = std::find_if(aScrambledPatterns.begin(), aScrambledPatterns.end(), [&aRet](const auto &pattern) {
        return ((std::popcount(pattern) == std::popcount(correctRepresentations[6])) &&
                ((pattern & aRet[1]) != aRet[1]));
    });
    AOC_Y2021_Day08_STORE_IT_OR_RETURN(6)

        const auto segC = (aRet[6] ^ aRet[8]) & segmentMask;
    const auto segEG = (aRet[6] & (~aRet[4])) & (~aRet[7]);
    const auto it0 =
        std::find_if(aScrambledPatterns.begin(), aScrambledPatterns.end(), [&segEG, &it6](const auto &pattern) {
            return ((std::popcount(pattern) == std::popcount(correctRepresentations[0])) && (pattern != (*it6)) &&
                    ((pattern & segEG) == segEG));
        });
    AOC_Y2021_Day08_STORE_IT_OR_RETURN(0)

        const auto it2 =
            std::find_if(aScrambledPatterns.begin(), aScrambledPatterns.end(), [&segEG](const auto &pattern) {
                return ((std::popcount(pattern) == std::popcount(correctRepresentations[2])) &&
                        ((pattern & segEG) == segEG));
            });
    AOC_Y2021_Day08_STORE_IT_OR_RETURN(2)

        const auto it3 =
            std::find_if(aScrambledPatterns.begin(), aScrambledPatterns.end(), [&aRet](const auto &pattern) {
                return ((std::popcount(pattern) == std::popcount(correctRepresentations[3])) &&
                        ((pattern & aRet[1]) == aRet[1]));
            });
    AOC_Y2021_Day08_STORE_IT_OR_RETURN(3)

        const auto it5 =
            std::find_if(aScrambledPatterns.begin(), aScrambledPatterns.end(), [&segC](const auto &pattern) {
                return ((std::popcount(pattern) == std::popcount(correctRepresentations[5])) &&
                        ((pattern & segC) == 0));
            });
    AOC_Y2021_Day08_STORE_IT_OR_RETURN(5)

        const auto it9 =
            std::find_if(aScrambledPatterns.begin(), aScrambledPatterns.end(), [&it0, &it6](const auto &pattern) {
                return ((std::popcount(pattern) == std::popcount(correctRepresentations[9])) && (pattern != (*it0)) &&
                        (pattern != (*it6)));
            });
    AOC_Y2021_Day08_STORE_IT_OR_RETURN(9)

#undef AOC_Y2021_Day08_STORE_IT_OR_RETURN

        return aRet;
}

AOC_Y2021_CONSTEXPR std::int64_t RestoreDisplayValue(const DisplayPatternAndValue &display)
{
    const auto aMapping = MapPatterns(display.first);
    if (std::any_of(aMapping.begin(), aMapping.end(), [](const auto &pattern) { return pattern == 0; }))
    {
        return -__LINE__;
    }
    int64_t ret = 0;
    for (auto it = display.second.begin(); it != display.second.end(); ++it)
    {
        ret *= 10;
        const auto itMatch = std::find(aMapping.begin(), aMapping.end(), *it);
        if (itMatch == aMapping.end())
        {
            return -__LINE__;
        }
        const auto decoded = std::distance(aMapping.begin(), itMatch);
        ret += decoded;
    }
    return ret;
}

AOC_Y2021_CONSTEXPR std::int64_t AccumulateRestoredDisplayValues(const auto &displays)
{
    if (displays.size() < 1)
    {
        return -__LINE__;
    }
    return std::accumulate(displays.begin(), displays.end(), 0LL, [](const auto acc, const auto &display) {
        const auto restoredDisplayValue = RestoreDisplayValue(display);
        return acc + restoredDisplayValue;
    });
}

#if AOC_Y2021_CONSTEXPR_UNIT_TEST
namespace
{
[[maybe_unused]] consteval void TestDay08()
{
    constexpr auto shortExample =
        "acedgfb cdfbe gcdfa fbcad dab cefabd cdfgeb eafb cagedb ab | cdfeb fcadb cdfeb cdbaf";
    constexpr auto longExample =
        "be cfbegad cbdgef fgaecd cgeb fdcge agebfd fecdb fabcd edb | fdgacbe cefdb cefbgd gcbe\nedbfga begcd cbg gc "
        "gcadebf fbgde acbgfd abcde gfcbed gfec | fcgedb cgb dgebacf gc\nfgaebd cg bdaec gdafb agbcfd gdcbef bgcad "
        "gfac gcb cdgabef | cg cg fdcagb cbg\nfbegcd cbd adcefb dageb afcb bc aefdc ecdab fgdeca fcdbega | efabcd "
        "cedba gadfec cb\naecbfdg fbg gf bafeg dbefa fcge gcbea fcaegb dgceab fcbdga | gecf egdcabf bgf bfgea\nfgeab "
        "ca afcebg bdacfeg cfaedg gcfdb baec bfadeg bafgc acf | gebdcfa ecba ca fadegcb\ndbcfg fgd bdegcaf fgec aegbdf "
        "ecdfab fbedc dacgb gdcebf gf | cefg dcbef fcge gbcadfe\nbdfegc cbegaf gecbf dfcage bdacg ed bedf ced adcbefg "
        "gebcd | ed bcgafe cdgba cbgef\negadfb cdbfeg cegd fecab cgb gbdefca cg fgcdab egfdb bfceg | gbdfcae bgc cg "
        "cgb\ngcafb gcf dcaebfg ecagb gf abcdeg gaef cafbge fdbac fegbdc | fgae cfgab fg bagce";
    static_assert(CountEasilyTdentifiableDigits(ParseInput(longExample)) == 26,
                  "Day 8 part 1: error calculating result");

    constexpr auto aMapping = MapPatterns(ParseInput(shortExample)[0].first);
    static_assert(aMapping[0] == ParseSegmentRepresentation("cagedb"), "Day 8 part 2: cannot map digit");
    static_assert(aMapping[1] == ParseSegmentRepresentation("ab"), "Day 8 part 2: cannot map digit");
    static_assert(aMapping[2] == ParseSegmentRepresentation("gcdfa"), "Day 8 part 2: cannot map digit");
    static_assert(aMapping[3] == ParseSegmentRepresentation("fbcad"), "Day 8 part 2: cannot map digit");
    static_assert(aMapping[4] == ParseSegmentRepresentation("eafb"), "Day 8 part 2: cannot map digit");
    static_assert(aMapping[5] == ParseSegmentRepresentation("cdfbe"), "Day 8 part 2: cannot map digit");
    static_assert(aMapping[6] == ParseSegmentRepresentation("cdfgeb"), "Day 8 part 2: cannot map digit");
    static_assert(aMapping[7] == ParseSegmentRepresentation("dab"), "Day 8 part 2: cannot map digit");
    static_assert(aMapping[8] == ParseSegmentRepresentation("acedgfb"), "Day 8 part 2: cannot map digit");
    static_assert(aMapping[9] == ParseSegmentRepresentation("cefabd"), "Day 8 part 2: cannot map digit");

    static_assert(AccumulateRestoredDisplayValues(ParseInput(longExample)) == 61229,
                  "Day 8 part 2: error calculating result");
}
} // namespace
#endif // AOC_Y2021_CONSTEXPR_UNIT_TEST

} // namespace

} // namespace Day08

class PuzzleDay08Impl final
{

  public:
    explicit AOC_Y2021_CONSTEXPR PuzzleDay08Impl(std::string_view input) : displays(Day08::ParseInput(input))
    {
    }

  public:
    const std::vector<Day08::DisplayPatternAndValue> displays;
};

AOC_Y2021_PUZZLE_CLASS_DECLARATION(08)

PuzzleDay08::PuzzleDay08(const std::string_view input) : pImpl(std::make_unique<PuzzleDay08Impl>(input))
{
}
PuzzleDay08::~PuzzleDay08() = default;

[[nodiscard]] IPuzzle::Solution_t PuzzleDay08::Part1()
{
    IPuzzle::Solution_t ret = std::monostate{};
    if (!pImpl || pImpl->displays.empty())
    {
        return ret;
    }
    const auto result = Day08::CountEasilyTdentifiableDigits(pImpl->displays);
    if (result >= 0)
    {
        ret = result;
    }
    return ret;
}

[[nodiscard]] IPuzzle::Solution_t PuzzleDay08::Part2()
{
    IPuzzle::Solution_t ret = std::monostate{};
    if (!pImpl || pImpl->displays.empty())
    {
        return ret;
    }
    const auto result = Day08::AccumulateRestoredDisplayValues(pImpl->displays);
    if (result >= 0)
    {
        ret = result;
    }
    return ret;
}

} // namespace AOC::Y2021
