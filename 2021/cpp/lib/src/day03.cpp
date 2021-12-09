#include "ipuzzle.hpp"
#include "puzzle_common.hpp"
#include "utils.hpp"

#include <algorithm>
#include <memory>
#include <numeric>
#include <optional>
#include <ranges>
#include <string_view>
#include <vector>

namespace AOC::Y2021
{

namespace
{

AOC_Y2021_CONSTEXPR std::vector<uint32_t> ParseInput(std::string_view input)
{
    std::vector<uint32_t> parsedInput;
    auto inputSize = input.size();
    const auto patternLen = input.find_first_of("\r\n");
    parsedInput.push_back(static_cast<uint32_t>(patternLen));
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
    return parsedInput;
}

AOC_Y2021_CONSTEXPR std::int64_t CalculatePowerConsumption(const std::vector<uint32_t> &input)
{
    const auto patternLen = input[0u];
    const auto inputCnt = input.size() - 1u;
    uint32_t gamma = 0u;
    for (size_t index = 0; index != patternLen; index++)
    {
        const auto numOfSetBits =
            std::count_if(input.begin() + 1u, input.end(), [&index](const auto &i) { return (i & (1u << index)); });
        const auto numOfClearedBits = inputCnt - numOfSetBits;

        if (static_cast<std::size_t>(numOfSetBits) > numOfClearedBits)
        {
            gamma |= 1u << index;
        }
    }
    const uint32_t epsilon = (~gamma) & ((1ul << patternLen) - 1ul);
    return gamma * epsilon;
}

AOC_Y2021_CONSTEXPR std::int64_t CalculateLifeSupportRating(const std::vector<uint32_t> &input)
{
    if (input.size() < 3)
    {
        return -__LINE__;
    }
    enum BitCriteria
    {
        BIT_CRITERIA_MOST_COMMON,
        BIT_CRITERIA_LEAST_COMMON,
        BIT_CRITERIA_COUNT,
    };
    auto fnFilterRating = [&input](BitCriteria bitCriteria) -> std::optional<std::uint32_t> {
        if (input.size() < 2)
        {
            return std::nullopt;
        }
        const bool bLookForMostCommon = (bitCriteria == BIT_CRITERIA_MOST_COMMON) ? true : false;
        const auto patternLen = input[0u];
        auto bitPos = patternLen - 1u;
        std::vector filtered(input.begin() + 1, input.end());
        while (filtered.size() > 1)
        {
            const auto bitMask = 1ul << bitPos;
            const auto numOfSetBits = static_cast<size_t>(
                std::ranges::count_if(filtered, [&bitMask](const auto &i) -> bool { return (i & bitMask); }));
            const auto numOfClearedBits = filtered.size() - numOfSetBits;
            const std::uint32_t commonBit =
                bLookForMostCommon ? (numOfSetBits >= numOfClearedBits) : (numOfSetBits < numOfClearedBits);
            std::erase_if(filtered,
                          [commonBit, bitPos](const auto &n) { return (((n >> bitPos) & 0b1u) != commonBit); });
            /* not supported by clang 12
            filtered = filtered | std::ranges::views::filter([commonBit, bitPos](const auto &n) {
                return (((n >> bitPos) & 0b1u) != commonBit);
            });
            */
            if (bitPos == 0)
            {
                break;
            }
            bitPos--;

            // filtered = filtered | std:git :ranges::views::filter([]
        }
        if (filtered.size() != 1)
        {
            return std::nullopt;
        }
        return filtered[0u];
    };
    const auto oxygenRating = fnFilterRating(BIT_CRITERIA_MOST_COMMON);
    if (!oxygenRating.has_value())
    {
        return -__LINE__;
    }
    const auto CO2Rating = fnFilterRating(BIT_CRITERIA_LEAST_COMMON);
    if (!CO2Rating.has_value())
    {
        return -__LINE__;
    }
    const auto lifeSupportRating = oxygenRating.value() * CO2Rating.value();
    return lifeSupportRating;
}

} // namespace

class PuzzleDay03Impl final
{

  public:
    AOC_Y2021_CONSTEXPR PuzzleDay03Impl(std::string_view input) : parsedInput(ParseInput(input))
    {
    }

  public:
    std::vector<uint32_t> parsedInput;
};
AOC_Y2021_PUZZLE_CLASS_DECLARATION(03)

PuzzleDay03::PuzzleDay03(const std::string_view input) : pImpl(std::make_unique<PuzzleDay03Impl>(input))
{
}

PuzzleDay03::~PuzzleDay03() = default;

std::optional<std::int64_t> PuzzleDay03::Part1()
{
    if (!pImpl || (pImpl->parsedInput.size() < 2))
    {
        return std::nullopt;
    }
    return CalculatePowerConsumption(pImpl->parsedInput);
}

std::optional<std::int64_t> PuzzleDay03::Part2()
{
    if (!pImpl || (pImpl->parsedInput.size() <= 2))
    {
        return std::nullopt;
    }
    const auto lifeSupportRating = CalculateLifeSupportRating(pImpl->parsedInput);
    if (lifeSupportRating < 0)
    {
        return std::nullopt;
    }
    return lifeSupportRating;
}

#if AOC_Y2021_CONSTEXPR_UNIT_TEST
namespace
{

[[maybe_unused]] consteval void TestDay03()
{
    constexpr std::string_view exampleInput =
        "00100\n11110\r\n10110\r\n10111\r\n10101\r\n01111\r\n00111\r\n11100\n10000\r\n11001\r\n00010\r\n01010\r";
    constexpr auto power = CalculatePowerConsumption(ParseInput(exampleInput));
    static_assert(power == 198, "Day 03 Part 1: Error calculating power consumption");
    constexpr auto lifeSupportRating = CalculateLifeSupportRating(ParseInput(exampleInput));
    static_assert(lifeSupportRating == 230, "Day 03 Part 2: Error calculating life support rating");
}

} // namespace
#endif // AOC_Y2021_CONSTEXPR_UNIT_TEST
} // namespace AOC::Y2021
