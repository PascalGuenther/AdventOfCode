#include "ipuzzle.hpp"
#include "puzzle_common.hpp"
#include "utils.hpp"

#include <algorithm>
#include <array>
#include <memory>
#include <numeric>
#include <optional>
#include <ranges>
#include <string_view>
#include <utility>
#include <vector>

namespace AOC::Y2021
{

namespace
{
class OceanFloor final
{
  public:
    class Line final
    {
      public:
        using Coordinate = std::pair<std::uint32_t, std::uint32_t>;

      public:
        AOC_Y2021_CONSTEXPR bool IsHorizontal() const
        {
            return (startCoord.second == endCoord.second);
        }

        AOC_Y2021_CONSTEXPR bool IsVertical() const
        {
            return (startCoord.first == endCoord.first);
        }

      public:
        Coordinate startCoord{};
        Coordinate endCoord{};
    };

  public:
    AOC_Y2021_CONSTEXPR static std::vector<Line> ParseInput(std::string_view input)
    {
        std::vector<Line> ret;
        while (input.size())
        {
            constexpr std::string_view arrow{"->"};
            const auto arrowPos = input.find(arrow);
            const auto endOfLine = input.find('\n');
            if ((arrowPos == input.npos) || (arrowPos >= endOfLine) || (arrowPos >= input.size()))
            {
                return {};
            }
            const auto arrowStr = input.substr(arrowPos);
            if (arrowStr.size() <= arrow.size())
            {
                return {};
            }
            const std::array<std::string_view, 2u> aPointStrs = {input.substr(0u, arrowPos),
                                                                 input.substr(arrowPos + arrow.size())};
            bool bFirst = true;
            decltype(Line::Coordinate::first) x1 = 0, x2 = 0;
            decltype(Line::Coordinate::second) y1 = 0, y2 = 0;
            for (const auto &str : aPointStrs)
            {
                const auto delimiterPos = str.find(',');
                if (delimiterPos == str.npos)
                {
                    return {};
                }
                const auto xStr = str.substr(0, delimiterPos);
                const auto yStr = str.substr(delimiterPos + 1);
                if ((xStr.size() == 0) || (yStr.size() == 0))
                {
                    return {};
                }
                if (bFirst)
                {
                    x1 = ParseNumber<std::uint32_t>(xStr);
                    y1 = ParseNumber<std::uint32_t>(yStr);
                    bFirst = false;
                }
                else
                {
                    x2 = ParseNumber<std::uint32_t>(xStr);
                    y2 = ParseNumber<std::uint32_t>(yStr);
                }
            }
            ret.push_back(Line{{x1, y1}, {x2, y2}});

            if (endOfLine == input.npos)
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

    AOC_Y2021_CONSTEXPR static auto GetGridDimensions(const auto &gridLines)
    {
        if (gridLines.empty())
        {
            return std::pair<decltype(Line::Coordinate::first), decltype(Line::Coordinate::second)>{};
        }
        const auto startPointXMax =
            std::max_element(gridLines.begin(), gridLines.end(), [](const auto &lhs, const auto &rhs) {
                return lhs.startCoord.first < rhs.startCoord.first;
            })->startCoord.first;
        const auto startPointYMax =
            std::max_element(gridLines.begin(), gridLines.end(), [](const auto &lhs, const auto &rhs) {
                return lhs.startCoord.second < rhs.startCoord.second;
            })->startCoord.second;
        const auto endPointXMax =
            std::max_element(gridLines.begin(), gridLines.end(), [](const auto &lhs, const auto &rhs) {
                return lhs.endCoord.first < rhs.endCoord.first;
            })->endCoord.first;
        const auto endPointYMax =
            std::max_element(gridLines.begin(), gridLines.end(), [](const auto &lhs, const auto &rhs) {
                return lhs.endCoord.second < rhs.endCoord.second;
            })->endCoord.second;

        return std::make_pair(std::max(startPointXMax, endPointXMax) + 1, std::max(startPointYMax, endPointYMax) + 1);
    }

    AOC_Y2021_CONSTEXPR static std::int64_t CalculateOverlappingPoints(const auto &lines,
                                                                       const bool bOnlyHorizontalOrVerticalLines = true)
    {
        const auto gridDimensions = GetGridDimensions(lines);
        std::vector<uint8_t> grid(gridDimensions.first * gridDimensions.second);
        for (const auto &line : lines)
        {
            if (bOnlyHorizontalOrVerticalLines)
            {
                if (!line.IsVertical() && !line.IsHorizontal())
                {
                    continue;
                }
            }
            const auto fnFlattenCoordinate = [gridDimensions](const Line::Coordinate &co) -> std::size_t {
                return std::size_t{(gridDimensions.first * co.second) + co.first};
            };
            const size_t startCoordFlat{fnFlattenCoordinate(line.startCoord)};
            const size_t endCoordFlat{fnFlattenCoordinate(line.endCoord)};
            const bool bSwap = endCoordFlat < startCoordFlat;
            const auto itStart = grid.begin() + (bSwap ? endCoordFlat : startCoordFlat);
            const auto itEnd = grid.begin() + (bSwap ? startCoordFlat : endCoordFlat);
            if (itEnd >= grid.end())
            {
                return -__LINE__;
            }
            const auto advance = [&line, &bSwap, &gridDimensions]() -> std::size_t {
                if (line.IsHorizontal())
                {
                    return 1;
                }
                else if (line.IsVertical())
                {
                    return gridDimensions.first;
                }
                else
                {
                    const auto &startCo = bSwap ? line.endCoord : line.startCoord;
                    const auto &endCo = bSwap ? line.startCoord : line.endCoord;
                    const bool bGoDownLeft = (startCo.first > endCo.first);
                    return bGoDownLeft ? (gridDimensions.first - 1) : (gridDimensions.first + 1);
                }
            }();
            // for (auto it = grid.begin(); it <= grid.end(); it += advance)
            for (auto it = itStart; it <= itEnd; it += advance)
            {
                *it += 1;
            }
        }
        return std::ranges::count_if(grid, [](const auto &overlappingLines) { return overlappingLines >= 2; });
    }

    OceanFloor() = delete;
    OceanFloor(const OceanFloor &) = delete;
    OceanFloor(OceanFloor &&) = delete;
};

} // namespace

class PuzzleDay05Impl final
{

  public:
    AOC_Y2021_CONSTEXPR PuzzleDay05Impl(std::string_view input) : floorLines(OceanFloor::ParseInput(input))
    {
    }

  public:
    const std::vector<OceanFloor::Line> floorLines;
};
AOC_Y2021_PUZZLE_CLASS_DECLARATION(05)

PuzzleDay05::PuzzleDay05(const std::string_view input) : pImpl(std::make_unique<PuzzleDay05Impl>(input))
{
}

PuzzleDay05::~PuzzleDay05() = default;

std::optional<std::int64_t> PuzzleDay05::Part1()
{
    if (!pImpl || pImpl->floorLines.empty())
    {
        return std::nullopt;
    }
    const auto result = OceanFloor::CalculateOverlappingPoints(pImpl->floorLines);
    if (result < 0)
    {
        return std::nullopt;
    }
    return result;
}

std::optional<std::int64_t> PuzzleDay05::Part2()
{
    if (!pImpl || pImpl->floorLines.empty())
    {
        return std::nullopt;
    }
    const auto result = OceanFloor::CalculateOverlappingPoints(pImpl->floorLines, false);
    if (result < 0)
    {
        return std::nullopt;
    }
    return result;
}

#if AOC_Y2021_CONSTEXPR_UNIT_TEST
namespace
{

[[maybe_unused]] consteval void TestDay05()
{
    // clang-format off
    constexpr auto exampleInput = "0,9 -> 5,9\r\n8,0 -> 0,8\n9,4 -> 3,4\n2,2 -> 2,1\r\n7,0 -> 7,4\r\n6,4 -> 2,0\n0,9 -> 2,9\r\n3,4 -> 1,4\r\n0,0 -> 8,8\r\n5,5 -> 8,2";
    constexpr OceanFloor::Line expectedLines[] = {
        { {0, 9}, {5, 9} },
        { {8, 0}, {0, 8} },
        { {9, 4}, {3, 4} },
        { {2, 2}, {2, 1} },
        { {7, 0}, {7, 4} },
        { {6, 4}, {2, 0} },
        { {0, 9}, {2, 9} },
        { {3, 4}, {1, 4} },
        { {0, 0}, {8, 8} },
        { {5, 5}, {8, 2} },
    };
    // clang-format on
    constexpr auto parsedSize = OceanFloor::ParseInput(exampleInput).size();
    static_assert(parsedSize == std::size(expectedLines), "Day 5: input parsed returs wrong length");
    constexpr auto parsedLines = Vector2Array<OceanFloor::Line, parsedSize>(OceanFloor::ParseInput(exampleInput));
    static_assert(std::ranges::equal(
                      parsedLines, expectedLines, [](const auto &lhs, const auto &rhs) constexpr {
                          return ((lhs.startCoord == rhs.startCoord) && (lhs.endCoord == rhs.endCoord));
                      }),
                  "Day 5: failed to parse input");
    static_assert(OceanFloor::GetGridDimensions(parsedLines) == OceanFloor::Line::Coordinate{10, 10});
    static_assert(OceanFloor::CalculateOverlappingPoints(parsedLines) == 5, "Day 5 part 1: error calculating result");
    static_assert(OceanFloor::CalculateOverlappingPoints(parsedLines, false) == 12,
                  "Day 5 part 2: error calculating result");
}

} // namespace
#endif // AOC_Y2021_CONSTEXPR_UNIT_TEST
} // namespace AOC::Y2021
