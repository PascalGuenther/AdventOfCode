#include "ipuzzle.hpp"
#include "puzzle_common.hpp"
#include "utils.hpp"

#include <algorithm>
#include <array>
#include <cstddef>
#include <cstdint>
#include <memory>
#include <numeric>
#include <set>
#include <string_view>
#include <utility>
#include <vector>

namespace AOC::Y2021
{

namespace Day15
{

namespace
{

using RiskMap = Vector2D<std::uint8_t>;

AOC_Y2021_CONSTEXPR bool ValidateInput(const RiskMap &input)
{
    if ((input.size() < 4) || (input.width() < 3) || (input.height() < 3))
    {
        return false;
    }
    if (std::any_of(input.begin(), input.end(), [](const auto &i) { return i > 9; }))
    {
        return false;
    }
    return true;
}

AOC_Y2021_CONSTEXPR IPuzzle::Solution_t SearchLowestRiskPath(const RiskMap &riskMap)
{
    if (!ValidateInput(riskMap))
    {
        return std::monostate{};
    }
    const auto dijkstra = [](const auto &graph, std::size_t startIndex, const std::size_t destinationIndex) {
        if ((startIndex > graph.size()) || (destinationIndex > graph.size()))
        {
            return INT64_MAX;
        }
        struct Vertex
        {
            std::size_t index;
            std::int64_t distance;
            [[nodiscard]] constexpr bool operator<(const Vertex &other) const
            {
                if (this->distance != other.distance)
                {
                    return this->distance < other.distance;
                }
                else
                {
                    return this->index < other.index;
                }
            }
        };
        const std::size_t width = graph.width();
        const std::size_t height = graph.width();
        constexpr std::size_t startDistance = 0;
        std::vector<std::int64_t> distancesFromStart(graph.size(), INT64_MAX);
        distancesFromStart[startIndex] = 0;
        std::set<Vertex> searchQueue{
            {
                .index = startIndex,
                .distance = startDistance,
            },
        };
        while (!searchQueue.empty())
        {
            auto vertex = searchQueue.extract(searchQueue.begin());
            if (vertex.value().index == destinationIndex)
            {
                return vertex.value().distance;
            }
            constexpr std::array aSearchDirection = {
                std::make_pair<std::int8_t, std::int8_t>(-1, 0),
                std::make_pair<std::int8_t, std::int8_t>(1, 0),
                std::make_pair<std::int8_t, std::int8_t>(0, -1),
                std::make_pair<std::int8_t, std::int8_t>(0, 1),
            };
            for (const auto &neighborDirection : aSearchDirection)
            {
                const bool bIsFirstCol = ((vertex.value().index % width) == 0);
                if (bIsFirstCol && (neighborDirection.first < 0))
                {
                    continue;
                }

                const bool bIsLastCol = ((vertex.value().index % width) == (width - 1U));
                if (bIsLastCol && (neighborDirection.first > 0))
                {
                    continue;
                }

                const bool bIsFirstRow = (vertex.value().index < width);
                if (bIsFirstRow && (neighborDirection.second < 0))
                {
                    continue;
                }

                const bool bIsLastRow = (vertex.value().index >= ((height - 1U) * width));
                if (bIsLastRow && (neighborDirection.second > 0))
                {
                    continue;
                }

                const auto offset =
                    (static_cast<std::int64_t>(width) * neighborDirection.second) + neighborDirection.first;
                const auto idx_ = vertex.value().index;
                const auto neighborIndex = static_cast<std::size_t>(static_cast<std::int64_t>(idx_) + offset);
                const auto neighborDistanceFromStartNew = vertex.value().distance + graph[neighborIndex];
                const auto neighborDistanceFromStartOld = distancesFromStart[neighborIndex];
                if (neighborDistanceFromStartOld >= neighborDistanceFromStartNew)
                {
                    distancesFromStart[neighborIndex] = neighborDistanceFromStartNew;
                    searchQueue.erase({
                        .index = neighborIndex,
                        .distance = neighborDistanceFromStartOld,
                    });
                    searchQueue.insert({neighborIndex, neighborDistanceFromStartNew});
                }
                if (searchQueue.empty() && (neighborDirection == aSearchDirection.back()))
                {
                    break;
                }
            }
        }
        return distancesFromStart[destinationIndex];
    };
    constexpr std::size_t startIdx = 0;
    const std::size_t destIdx = riskMap.size() - 1u;
    const auto lowestRisk = dijkstra(riskMap, startIdx, destIdx);
    if (lowestRisk < INT64_MAX)
    {
        return lowestRisk;
    }
    else
    {
        return std::monostate{};
    }
}

AOC_Y2021_CONSTEXPR IPuzzle::Solution_t ExpandMapAndSearchLowRiskPath(const RiskMap &riskMap)
{
    constexpr std::size_t expansionFactor = 5;
    Vector2D expandedMap(riskMap.width() * expansionFactor,
                         std::vector<std::uint8_t>(expansionFactor * expansionFactor * riskMap.size(), 0));

    const auto transformValue = [](const auto &source, const std::size_t x, const std::size_t y,
                                   const std::uint8_t add) {
        const std::uint8_t original = source(x, y);
        std::uint8_t updated = original + add;
        if (updated > 9)
        {
            updated = static_cast<std::uint8_t>(((updated - 1U) % 9U) + 1U);
        }
        return updated;
    };
    for (std::uint8_t expandX = 0U; expandX != expansionFactor; ++expandX)
    {
        for (size_t x = 0; x != riskMap.width(); x++)
        {
            for (size_t y = 0; y != riskMap.height(); y++)
            {
                expandedMap((expandX * riskMap.width()) + x, y) = transformValue(riskMap, x, y, expandX);
            }
        }
    }

    for (std::uint8_t expandY = 1U; expandY != expansionFactor; ++expandY)
    {
        for (size_t x = 0U; x != expandedMap.width(); x++)
        {
            for (size_t y = 0U; y != riskMap.height(); y++)
            {
                expandedMap(x, (expandY * riskMap.height()) + y) = transformValue(expandedMap, x, y, expandY);
            }
        }
    }

    return SearchLowestRiskPath(expandedMap);
}

} // namespace

} // namespace Day15

class PuzzleDay15Impl final
{

  public:
    explicit AOC_Y2021_CONSTEXPR PuzzleDay15Impl(std::string_view input) : riskMap(ParseToVector2D(input))
    {
    }

  public:
    Day15::RiskMap riskMap;
};

#if defined(AOC_Y2021_PUZZLE_CLASS_DECLARATION)

AOC_Y2021_PUZZLE_CLASS_DECLARATION(15)

PuzzleDay15::PuzzleDay15(const std::string_view input) : pImpl(std::make_unique<PuzzleDay15Impl>(input))
{
}
PuzzleDay15::~PuzzleDay15() = default;

[[nodiscard]] IPuzzle::Solution_t PuzzleDay15::Part1()
{
    const auto result = Day15::SearchLowestRiskPath(pImpl->riskMap);
    return result;
}

[[nodiscard]] IPuzzle::Solution_t PuzzleDay15::Part2()
{
    const auto result = Day15::ExpandMapAndSearchLowRiskPath(pImpl->riskMap);
    return result;
}

#endif // defined(AOC_Y2021_PUZZLE_CLASS_DECLARATION)

} // namespace AOC::Y2021