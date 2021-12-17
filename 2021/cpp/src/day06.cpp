#include "ipuzzle.hpp"
#include "puzzle_common.hpp"
#include "utils.hpp"

#include <algorithm>
#include <array>
#include <cstddef>
#include <cstdint>
#include <memory>
#include <numeric>
#include <ranges>
#include <string_view>
#include <utility>

namespace AOC::Y2021
{

namespace
{

constexpr std::uint8_t counterMax{8u};
using PopulationHistogram = std::array<std::int64_t, counterMax + 1>;

AOC_Y2021_CONSTEXPR auto ParseInput(std::string_view input)
{
    PopulationHistogram population{};
    for (std::size_t start = 0; input.size(); input.remove_prefix(start + 1))
    {
        const auto counter = ParseNumber<std::uint32_t>(input);
        if (counter >= population.size())
        {
            break;
        }
        population[counter]++;
        start = input.find_first_of(',');
        if (start == input.npos)
        {
            break;
        }
    }
    return population;
}

template <std::size_t maxDays> consteval auto GeneratePopulationLUT()
{
    constexpr std::uint8_t counterRestart{6u};
    std::array<PopulationHistogram, maxDays + 1u> aCache{};
    auto fnOffspring = [&counterRestart, &aCache](const std::uint8_t counter, const std::uint64_t days,
                                                  auto &&recursion) -> std::int64_t {
        auto &cacheEntry = aCache[days][counter];
        if (cacheEntry < 1)
        {
            switch (days)
            {
            case 0:
                cacheEntry = 1;
                break;
            case 1:
                cacheEntry = (counter == 0) ? 2 : 1;
                break;
            default:
                cacheEntry =
                    (counter == 0)
                        ? (recursion(counterRestart, days - 1, recursion) + recursion(counterMax, days - 1, recursion))
                        : recursion(counter - 1, days - 1, recursion);
                break;
            }
        }
        return cacheEntry;
    };
    PopulationHistogram histogramMultiplierLUT{};
    std::iota(histogramMultiplierLUT.begin(), histogramMultiplierLUT.end(), 0u);
    for (auto &i : histogramMultiplierLUT)
    {
        i = fnOffspring(i, maxDays, fnOffspring);
    }
    return histogramMultiplierLUT;
}

template <std::size_t maxDays> AOC_Y2021_CONSTEXPR auto SimulatePopulation(const PopulationHistogram &initial)
{
    constexpr auto offspringLUT = GeneratePopulationLUT<maxDays>();
    return std::inner_product(initial.begin(), initial.end(), offspringLUT.begin(), 0LL);
}

} // namespace

class PuzzleDay06Impl final
{

  public:
    AOC_Y2021_CONSTEXPR PuzzleDay06Impl(std::string_view input) : fish(ParseInput(input))
    {
    }

  public:
    const PopulationHistogram fish;
};
AOC_Y2021_PUZZLE_CLASS_DECLARATION(06)

PuzzleDay06::PuzzleDay06(const std::string_view input) : pImpl(std::make_unique<PuzzleDay06Impl>(input))
{
}

PuzzleDay06::~PuzzleDay06() = default;

[[nodiscard]] IPuzzle::Solution_t PuzzleDay06::Part1()
{
    if (!pImpl)
    {
        return std::monostate{};
    }
    const auto result = SimulatePopulation<80>(pImpl->fish);
    if (result < 0)
    {
        return std::monostate{};
    }
    return result;
}

[[nodiscard]] IPuzzle::Solution_t PuzzleDay06::Part2()
{
    if (!pImpl)
    {
        return std::monostate{};
    }
    const auto result = SimulatePopulation<256>(pImpl->fish);
    if (result < 0)
    {
        return std::monostate{};
    }
    return result;
}

#if AOC_Y2021_CONSTEXPR_UNIT_TEST
namespace
{

[[maybe_unused]] consteval void TestDay06()
{
    constexpr auto exampleInput = "3,4,3,1,2";
    constexpr PopulationHistogram expectedPopulation = {
        0, 1, 1, 2, 1, 0, 0, 0, 0,
    };
    constexpr auto parsedPopulation = ParseInput(exampleInput);
    static_assert(std::ranges::equal(parsedPopulation, expectedPopulation), "Day 6: failed to parse input");
    static_assert(SimulatePopulation<80>(parsedPopulation) == 5'934LL, "Day 6 part 1: error calculating result");
    static_assert(SimulatePopulation<256>(parsedPopulation) == 26'984'457'539LL,
                  "Day 6 part 2: error calculating result");
}

} // namespace
#endif // AOC_Y2021_CONSTEXPR_UNIT_TEST
} // namespace AOC::Y2021
