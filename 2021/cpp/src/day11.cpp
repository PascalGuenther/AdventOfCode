#include "ipuzzle.hpp"
#include "puzzle_common.hpp"
#include "utils.hpp"

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <limits>
#include <memory>
#include <numeric>
#include <optional>
#include <string_view>
#include <utility>

namespace AOC::Y2021
{

class PuzzleDay11Impl final
{
  public:
    using OctopusMap = std::array<std::array<std::uint8_t, 10U>, 10U>;

  public:
    AOC_Y2021_CONSTEXPR static OctopusMap ParseInput(const std::string_view &input)
    {
        OctopusMap aaMap{};
        const auto fnValidChar = [](const auto &c) { return ((c >= '0') && (c <= '9')); };
        const auto validEntries = static_cast<std::size_t>(
            std::count_if(input.begin(), input.end(), [fnValidChar](const auto &c) { return fnValidChar(c); }));
        if (validEntries != (aaMap.size() * aaMap[0].size()))
        {
            return aaMap;
        }
        std::size_t index = 0u;
        for (const auto &c : input)
        {
            if (!fnValidChar(c))
            {
                continue;
            }
            auto &insertAt = aaMap[index / aaMap[0].size()][index % aaMap[0].size()];
            ++index;
            insertAt = static_cast<std::decay_t<decltype(insertAt)>>(c - '0');
        }
        return aaMap;
    }

    AOC_Y2021_CONSTEXPR PuzzleDay11Impl(const std::string_view &input) : octopusMap(ParseInput(input))
    {
    }

    AOC_Y2021_CONSTEXPR int64_t RunSteps(const std::uint16_t stepCount) const
    {
        std::int64_t accFlashes = 0;
        auto mapCopy = octopusMap;
        for (std::uint16_t step = 0; step != stepCount; ++step)
        {
            accFlashes += SimulateStep(mapCopy);
        }
        return accFlashes;
    }

    AOC_Y2021_CONSTEXPR int64_t FindStepInWhichAllAreFlashing() const
    {
        auto mapCopy = octopusMap;
        for (std::uint16_t step = 0; std::numeric_limits<decltype(step)>::max() - 1u; ++step)
        {
            const auto numOctopusses = mapCopy.size() * mapCopy[0].size();
            const auto numFlashingOctopussesInThisStep = SimulateStep(mapCopy);
            if (numOctopusses == numFlashingOctopussesInThisStep)
            {
                return step + 1;
            }
        }
        return -__LINE__;
    }

  private:
    AOC_Y2021_CONSTEXPR static std::uint32_t SimulateStep(OctopusMap &map)
    {
        for (auto &rowOfOctopusses : map)
        {
            for (auto &octopus : rowOfOctopusses)
            {
                octopus++;
            }
        }

        constexpr std::uint8_t flashThreshold = 10;
        constexpr std::uint8_t flashedMarker = 128;
        std::uint32_t flashes = 0;
        std::uint32_t prevFlashes = 0;
        do
        {
            prevFlashes = flashes;

            for (std::uint8_t row = 0u; row != map.size(); ++row)
            {

                for (std::uint8_t col = 0u; col != map[0].size(); ++col)
                {
                    auto &energyLevel = map[row][col];
                    bool flash = (energyLevel >= flashThreshold) && (energyLevel < flashedMarker);
                    if (flash)
                    {
                        energyLevel = flashedMarker;
                        flashes++;
                        const auto fnIncrementNeighbors = [&map](const std::uint32_t row, const std::uint32_t middleCol,
                                                                 const bool ignoreMiddle) {
                            if (middleCol > 0)
                            {
                                map[row][middleCol - 1u]++;
                            }
                            if (!ignoreMiddle)
                            {
                                map[row][middleCol]++;
                            }
                            const auto rightCol = middleCol + 1u;
                            if ((rightCol) < map[0].size())
                            {
                                map[row][rightCol]++;
                            }
                        };
                        if (row > 0)
                        {
                            fnIncrementNeighbors(row - 1u, col, false);
                        }
                        fnIncrementNeighbors(row, col, true);
                        const auto rowBelow = row + 1u;
                        if (rowBelow < map.size())
                        {
                            fnIncrementNeighbors(rowBelow, col, false);
                        }
                    }
                }
            }
        } while (flashes != prevFlashes);

        for (auto &rowOfOctopusses : map)
        {
            for (auto &octopus : rowOfOctopusses)
            {
                if (octopus >= flashThreshold)
                {
                    octopus = 0;
                }
            }
        }
        return flashes;
    }

  private:
    OctopusMap octopusMap;
};
namespace Day11
{

namespace
{

#if AOC_Y2021_CONSTEXPR_UNIT_TEST
namespace
{
[[maybe_unused]] consteval void Test()
{
    constexpr std::string_view exampleInput = "5483143223\n2745854711\n5264556173\n6141336146\n6357385478\n41675"
                                              "24645\n2176841721\n6882881134\n4846848554\n5283751526\n";
    constexpr PuzzleDay11Impl puzzle{exampleInput};
    static_assert(puzzle.RunSteps(100) == 1656, "Day 11 part 1: error calculating the number of flashes");
    static_assert(puzzle.FindStepInWhichAllAreFlashing() == 195,
                  "Day 11 part 2: error calculating the first step during which all octopusses are flashing");
}
} // namespace
#endif // AOC_Y2021_CONSTEXPR_UNIT_TEST

} // namespace

} // namespace Day11

AOC_Y2021_PUZZLE_CLASS_DECLARATION(11)

PuzzleDay11::PuzzleDay11(const std::string_view input) : pImpl(std::make_unique<PuzzleDay11Impl>(input))
{
}
PuzzleDay11::~PuzzleDay11() = default;

std::optional<std::int64_t> PuzzleDay11::Part1()
{
    if (!pImpl)
    {
        return std::nullopt;
    }
    const auto result = pImpl->RunSteps(100);
    return result;
}

std::optional<std::int64_t> PuzzleDay11::Part2()
{
    if (!pImpl)
    {
        return std::nullopt;
    }
    const auto result = pImpl->FindStepInWhichAllAreFlashing();
    if (result < 0)
    {
        return std::nullopt;
    }
    else
    {
        return result;
    }
}

} // namespace AOC::Y2021
