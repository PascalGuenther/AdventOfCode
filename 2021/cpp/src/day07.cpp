#include "ipuzzle.hpp"
#include "puzzle_common.hpp"
#include "utils.hpp"

#include <algorithm>
#include <array>
#include <cstddef>
#include <cstdint>
#include <memory>
#include <numeric>
#include <string_view>
#include <utility>
#include <vector>

namespace AOC::Y2021
{

namespace
{

AOC_Y2021_CONSTEXPR auto ParseInput(std::string_view input)
{
    const auto parsedPositions = ParseToVectorOfNums<std::uint32_t, ','>(input);
    const auto maxInput = *std::ranges::max_element(parsedPositions);
    std::vector<std::decay_t<decltype(maxInput)>> posistionHistogram(maxInput + 1);
    for (const auto &position : parsedPositions)
    {
        ++posistionHistogram[position];
    }
    return posistionHistogram;
}

AOC_Y2021_CONSTEXPR std::int64_t CalculateFuelConsumption(const auto &positionHistogram, const std::size_t position,
                                                          const bool bConstantFuelBurn)
{
    if (position >= positionHistogram.size())
    {
        return -__LINE__;
    }

    std::size_t index{}; /// Caution: Do NOT to parallelize this algo
    const auto fuel =
        std::accumulate(positionHistogram.begin(), positionHistogram.end(), 0u,
                        [&index, &position, &bConstantFuelBurn](const auto &acc, const auto &rhs) {
                            const auto n =
                                static_cast<uint32_t>((index > position) ? (index - position) : (position - index));
                            ++index;
                            const decltype(n) weightedFactor = (bConstantFuelBurn) ? n : ((n * (n + 1u)) / 2u);
                            const auto fuel = weightedFactor * rhs;
                            return acc + fuel;
                        });
    return fuel;
}

AOC_Y2021_CONSTEXPR std::int64_t CalculateCheapestFuelOption(const auto &posistionHistogram,
                                                             const bool bConstantFuelBurn)
{
    if (posistionHistogram.empty())
    {
        return -__LINE__;
    }

    auto minFuel = CalculateFuelConsumption(posistionHistogram, 0, bConstantFuelBurn);
    for (std::size_t pos = 1; pos != posistionHistogram.size(); ++pos)
    {
        const auto fuel = CalculateFuelConsumption(posistionHistogram, pos, bConstantFuelBurn);
        if (fuel < minFuel)
        {
            minFuel = fuel;
        }
    }
    return minFuel;
}

#if AOC_Y2021_CONSTEXPR_UNIT_TEST
namespace
{
[[maybe_unused]] consteval void TestDay07()
{
    constexpr auto exampleInput = "16,1,2,0,4,2,7,1,2,14";
    constexpr std::uint32_t expectedParsedInput[] = {
        1, 2, 3, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 1,
    };
    constexpr auto parsedSize = ParseInput(exampleInput).size();
    static_assert(parsedSize == (sizeof(expectedParsedInput) / sizeof(expectedParsedInput[0])),
                  "Day 7: failed to parse input due to wrong size");
    constexpr auto parsedInput = Vector2Array<std::uint32_t, parsedSize>(ParseInput(exampleInput));
    static_assert(std::equal(parsedInput.begin(), parsedInput.end(), std::begin(expectedParsedInput)),
                  "Day 7: failed to parse input");
    static_assert(CalculateCheapestFuelOption(parsedInput, true) == 37, "Day 7 part 1: error calculating result");
    static_assert(CalculateCheapestFuelOption(parsedInput, false) == 168, "Day 7 part 2: error calculating result");
}
} // namespace
#endif // AOC_Y2021_CONSTEXPR_UNIT_TEST

} // namespace

class PuzzleDay07Impl final
{

  public:
    explicit AOC_Y2021_CONSTEXPR PuzzleDay07Impl(std::string_view input) : crabPosHistogram(ParseInput(input))
    {
    }

  public:
    const std::vector<uint32_t> crabPosHistogram;
};

AOC_Y2021_PUZZLE_CLASS_DECLARATION(07)

PuzzleDay07::PuzzleDay07(const std::string_view input) : pImpl(std::make_unique<PuzzleDay07Impl>(input))
{
}
PuzzleDay07::~PuzzleDay07() = default;

[[nodiscard]] IPuzzle::Solution_t PuzzleDay07::Part1()
{
    if (!pImpl || pImpl->crabPosHistogram.empty())
    {
        return std::monostate{};
    }
    return CalculateCheapestFuelOption(pImpl->crabPosHistogram, true);
}

[[nodiscard]] IPuzzle::Solution_t PuzzleDay07::Part2()
{
    if (!pImpl || pImpl->crabPosHistogram.empty())
    {
        return std::monostate{};
    }
    return CalculateCheapestFuelOption(pImpl->crabPosHistogram, false);
}

} // namespace AOC::Y2021
