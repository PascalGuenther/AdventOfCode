#include "ipuzzle.hpp"
#include "puzzle_common.hpp"
#include "utils.hpp"

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <memory>
#include <numeric>
#include <string_view>
#include <utility>
#include <vector>

namespace AOC::Y2021
{
namespace Day09
{

namespace
{

constexpr std::uint8_t notInBasin{9};

using HeightMap = Vector2D<std::uint8_t>;
using LowPointMap = std::vector<std::pair<std::size_t, std::size_t>>;

AOC_Y2021_CONSTEXPR bool ValidateInput(const HeightMap &input)
{
    if ((input.size() < 1) || (input.width() < 1) || (input.height() < 1))
    {
        return false;
    }
    if (std::any_of(input.begin(), input.end(), [](const auto &i) { return i > 9; }))
    {
        return false;
    }
    return true;
}

AOC_Y2021_CONSTEXPR LowPointMap FindLowPoints(const HeightMap &input)
{
    LowPointMap lowPoints{};
    if (!ValidateInput(input))
    {
        return lowPoints;
    }
    const auto width = input.width();
    if (width < 1)
    {
        return lowPoints;
    }
    const auto height = input.height();
    if (height < 1)
    {
        return lowPoints;
    }
    for (std::size_t row = 0; row != height; ++row)
    {
        const bool bIsFirstRow = (row == 0);
        const bool bIsLastRow = (row == (height - 1));
        for (std::size_t col = 0; col != width; ++col)
        {
            const bool bIsFirstCol = (col == 0);
            const bool bIsLastCol = (col == (width - 1));
            const auto i = input(col, row);
            if (!bIsFirstRow && (i >= input(col, row - 1)))
            {
                continue;
            }
            if (!bIsLastRow && (i >= input(col, row + 1)))
            {
                continue;
            }
            if (!bIsFirstCol && (i >= input(col - 1, row)))
            {
                continue;
            }
            if (!bIsLastCol && (i >= input(col + 1, row)))
            {
                continue;
            }
            lowPoints.emplace_back(col, row);
        }
    }
    return lowPoints;
}

AOC_Y2021_CONSTEXPR std::int64_t CalculateRiskLevel(const HeightMap &input,
                                                    const std::pair<std::size_t, std::size_t> &col_row)
{
    const auto [col, row] = col_row;
    if (!ValidateInput(input) || (col >= input.width()) || (row >= input.height()))
    {
        return -__LINE__;
    }
    return input(col, row) + 1;
}

AOC_Y2021_CONSTEXPR std::int64_t AccumulatedRiskLevelOfLowPoints(const HeightMap &input, const auto &lowPoints)
{
    return std::accumulate(lowPoints.begin(), lowPoints.end(), 0LL, [&input](const auto &acc, const auto &col_row) {
        const auto riskLevel = CalculateRiskLevel(input, col_row);
        return acc + riskLevel;
    });
    return 0;
}

template <std::size_t N> AOC_Y2021_CONSTEXPR auto FindLargestBasins(const HeightMap &input, const auto &lowPoints)
{
    std::array<std::int64_t, N> aBasins{};
    if (!ValidateInput(input))
    {
        return aBasins;
    }
    auto annotatedMap{input};
    const auto width = annotatedMap.width();
    const auto height = annotatedMap.height();
    if ((width < 1) || height < 1)
    {
        return aBasins;
    }
    const auto fnGrowBasin = [&annotatedMap, width, height](const std::size_t col, const std::size_t row,
                                                            const std::uint8_t compareTo, auto &&recursion) {
        std::int64_t ret{0};
        const auto here = annotatedMap(col, row);
        if ((here == notInBasin) || (here < compareTo))
        {
            return ret;
        }
        annotatedMap(col, row) = notInBasin;
        ++ret;
        const bool bIsFirstRow = (row == 0);
        const bool bIsLastRow = (row == (height - 1));
        const bool bIsFirstCol = (col == 0);
        const bool bIsLastCol = (col == (width - 1));
        if (!bIsFirstRow)
        {
            ret += recursion(col, row - 1, here, recursion);
        }
        if (!bIsLastRow)
        {
            ret += recursion(col, row + 1, here, recursion);
        }
        if (!bIsFirstCol)
        {
            ret += recursion(col - 1, row, here, recursion);
        }
        if (!bIsLastCol)
        {
            ret += recursion(col + 1, row, here, recursion);
        }
        return ret;
    };
    for (const auto &[col, row] : lowPoints)
    {
        const auto basinArea = fnGrowBasin(col, row, 0, fnGrowBasin);
        auto itMin = std::min_element(aBasins.begin(), aBasins.end());
        if (itMin == aBasins.end())
        {
            aBasins.fill(0);
            return aBasins;
        }
        if ((*itMin) < basinArea)
        {
            *itMin = basinArea;
        }
    }
    return aBasins;
}

template <std::size_t N>
AOC_Y2021_CONSTEXPR std::int64_t MultiplyBasinAreas(const HeightMap &input, const auto &lowPoints)
{
    if (lowPoints.size() < N)
    {
        return -__LINE__;
    }
    const auto aBasinAreas = FindLargestBasins<N>(input, lowPoints);
    const auto multipliedAreas = std::reduce(aBasinAreas.begin(), aBasinAreas.end(), 1LL, std::multiplies<int>());
    return multipliedAreas;
}

#if AOC_Y2021_CONSTEXPR_UNIT_TEST
namespace
{
[[maybe_unused]] consteval auto Test()
{
    constexpr auto exampleInput = "2199943210\n3987894921\n9856789892\n8767896789\n9899965678";
    constexpr std::array<std::pair<std::size_t, std::size_t>, 4U> expectedLowPoints{{{1, 0}, {9, 0}, {2, 2}, {6, 4}}};
    constexpr auto parsedLowPoints =
        Vector2Array<std::pair<std::size_t, std::size_t>, FindLowPoints(ParseToVector2D(exampleInput)).size()>(
            FindLowPoints(ParseToVector2D(exampleInput)));
    static_assert(expectedLowPoints.size() == parsedLowPoints.size(), "Day 9 part 1: incorrect number of low points");
    static_assert(std::equal(parsedLowPoints.begin(), parsedLowPoints.end(), expectedLowPoints.begin()),
                  "Day 9 part 1: error identifying low points");
    static_assert(AccumulatedRiskLevelOfLowPoints(ParseToVector2D(exampleInput), parsedLowPoints) == 15,
                  "Day 9 part 1: error calculating result");
    static_assert(MultiplyBasinAreas<3>(ParseToVector2D(exampleInput), parsedLowPoints) == 1134,
                  "Day 9 part 2: error calculating result");
}
} // namespace
#endif // AOC_Y2021_CONSTEXPR_UNIT_TEST

} // namespace

} // namespace Day09

class PuzzleDay09Impl final
{

  public:
    AOC_Y2021_CONSTEXPR PuzzleDay09Impl(std::string_view input) : parsedInput(ParseToVector2D(input))
    {
    }

  public:
    Day09::HeightMap parsedInput;
    Day09::LowPointMap lowPointMap;
};

#if defined(AOC_Y2021_PUZZLE_CLASS_DECLARATION)

AOC_Y2021_PUZZLE_CLASS_DECLARATION(09)

PuzzleDay09::PuzzleDay09(const std::string_view input) : pImpl(std::make_unique<PuzzleDay09Impl>(input))
{
}
PuzzleDay09::~PuzzleDay09() = default;

[[nodiscard]] IPuzzle::Solution_t PuzzleDay09::Part1()
{
    IPuzzle::Solution_t ret = std::monostate{};
    if (!pImpl || pImpl->parsedInput.empty())
    {
        return ret;
    }
    pImpl->lowPointMap = Day09::FindLowPoints(pImpl->parsedInput);
    const auto result = Day09::AccumulatedRiskLevelOfLowPoints(pImpl->parsedInput, pImpl->lowPointMap);
    if (result >= 0)
    {
        ret = result;
    }
    return result;
}

[[nodiscard]] IPuzzle::Solution_t PuzzleDay09::Part2()
{
    IPuzzle::Solution_t ret = std::monostate{};
    if (!pImpl || pImpl->parsedInput.empty())
    {
        return ret;
    }
    if (pImpl->lowPointMap.empty())
    {
        pImpl->lowPointMap = Day09::FindLowPoints(pImpl->parsedInput);
    }
    pImpl->lowPointMap = Day09::FindLowPoints(pImpl->parsedInput);
    const auto result = Day09::MultiplyBasinAreas<3>(pImpl->parsedInput, pImpl->lowPointMap);
    if (result >= 0)
    {
        ret = result;
    }
    return ret;
}

#endif // defined(AOC_Y2021_PUZZLE_CLASS_DECLARATION)

} // namespace AOC::Y2021
