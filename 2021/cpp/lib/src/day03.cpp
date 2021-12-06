#include "ipuzzle.hpp"
#include "puzzle_common.hpp"
#include "utils.hpp"

#include <memory>
#include <numeric>
#include <optional>
#include <string_view>
#include <vector>

#include <iostream>

namespace AOC::Y2021
{

namespace
{
struct Rates
{
    uint32_t gamma;
    uint32_t epsilon;
};
AOC_Y2021_CONSTEXPR Rates ParseInput(std::string_view input)
{
    Rates ret{};
    std::vector<uint32_t> parsedInput;
    auto inputSize = input.size();
    const auto patternLen = input.find_first_of("\r\n");
    while (inputSize > 4u)
    {
        parsedInput.push_back(ParseNumber<uint32_t>(input, 2));
        const auto endOfLine = input.find('\n');
        if (endOfLine == input.npos)
        {
            break;
        }
        const auto startOfNextLine = endOfLine + 1u;
        if (startOfNextLine >= inputSize)
        {
            break;
        }
        input.remove_prefix(startOfNextLine);
        inputSize = input.size();
    }
    const auto inputCnt = parsedInput.size();
    for (size_t index = 0; index != 32; index++)
    {
        const auto numOfSetBits = std::count_if(parsedInput.begin(), parsedInput.end(),
                                                [&index](const auto &i) { return (i & (1u << index)); });
        const auto numOfClearedBits = inputCnt - numOfSetBits;

        if (static_cast<std::size_t>(numOfSetBits) > numOfClearedBits)
        {
            ret.gamma |= 1u << index;
        }
    }
    ret.epsilon = (~ret.gamma) & ((1ul << patternLen) - 1ul);
    return ret;
}

AOC_Y2021_CONSTEXPR std::int64_t Part01(std::string_view input)
{
    const auto rates = ParseInput(input);
    return rates.gamma * rates.epsilon;
}
} // namespace

class PuzzleDay03Impl final
{

  public:
    AOC_Y2021_CONSTEXPR PuzzleDay03Impl(std::string_view input) : input(input)
    {
    }

  public:
    std::string input;
};
AOC_Y2021_PUZZLE_CLASS_DECLARATION(03)

PuzzleDay03::PuzzleDay03(const std::string_view input) : pImpl(std::make_unique<PuzzleDay03Impl>(input))
{
}

PuzzleDay03::~PuzzleDay03() = default;

std::optional<std::int64_t> PuzzleDay03::Part1()
{
    if (!pImpl || (pImpl->input.size() < 2))
    {
        return std::nullopt;
    }
    return Part01(pImpl->input);
}

std::optional<std::int64_t> PuzzleDay03::Part2()
{
    if (!pImpl || (pImpl->input.size() <= 2))
    {
        return std::nullopt;
    }
    return std::nullopt; // return pImpl->Part02();
}

#if AOC_Y2021_CONSTEXPR_UNIT_TEST
namespace
{
constexpr std::string_view exampleInput =
    "00100\n11110\r\n10110\r\n10111\r\n10101\r\n01111\r\n00111\r\n11100\n10000\r\n11001\r\n00010\r\n01010\r";

[[maybe_unused]] consteval void TestDay03Part1()
{
    constexpr auto rates = ParseInput(exampleInput);
    static_assert(rates.gamma == 22, "Day 03 Part 1: Error calculating gamma rate");
    static_assert(rates.epsilon == 9, "Day 03 Part 1: Error calculating epsilon rate");
    static_assert(Part01(exampleInput) == 198, "Day 03 Part 1: Error calculating power consumption");
}

} // namespace
#endif // AOC_Y2021_CONSTEXPR_UNIT_TEST
} // namespace AOC::Y2021
