#include "ipuzzle.hpp"
#include "puzzle_common.hpp"
#include "utils.hpp"

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <memory>
#include <numeric>
#include <ranges>
#include <string_view>
#include <utility>
#include <vector>

namespace AOC::Y2021
{

namespace Day12
{

namespace
{

struct Graph
{
    std::unique_ptr<char[]> str;
    std::vector<std::pair<std::string_view, std::string_view>> edges;
};

Graph ParseInput(const std::string_view &input)
{
    Graph parsed = {
        .str = std::make_unique<char[]>(input.size() + 1),
        .edges = {},
    };
    input.copy(parsed.str.get(), input.size());
    std::string_view copiedInput(parsed.str.get(), input.size());
    auto forEachLine = [&parsed](const std::string_view &line) -> bool {
        constexpr char delimiter = '-';
        bool foundDelimiter = false;
        for (const auto &c : line)
        {
            if (c == delimiter)
            {
                if (foundDelimiter)
                {
                    return false;
                }
                foundDelimiter = true;
            }
            else if (!(((c >= 'a') && (c <= 'z')) || ((c >= 'A') && (c <= 'Z'))))
            {
                return false;
            }
        }
        const auto delimiterPos = line.find(delimiter);
        if ((delimiterPos == 0) || (delimiterPos == line.npos) || ((delimiterPos + 1u) == line.size()))
        {
            return false;
        }
        parsed.edges.push_back(
            std::make_pair(std::string_view{line.substr(0, delimiterPos)}, std::string_view{line.substr(delimiterPos + 1)}));
        return true;
    };
    if (!ParseLines(copiedInput, forEachLine))
    {
        return {};
    }
    else
    {
        return parsed;
    }
}

AOC_Y2021_CONSTEXPR std::int64_t CountPaths(const auto &edges, bool aSingleSmallOnceMayBeVisitedTwice)
{
    std::vector<decltype(edges[0].first)> path;
    path.emplace_back("start");
    std::uint32_t canary = 10'000'000;
    const auto countPaths = [&edges, &aSingleSmallOnceMayBeVisitedTwice, &canary](const auto &prevPath,
                                                                                  auto &&recursion) -> std::int64_t {
        std::int64_t pathsCnt = 0;
        const auto &vertex = prevPath.back();
        for (const auto &[lhs, rhs] : edges)
        {
            if (!(--canary))
            {
                return -__LINE__;
            }
            if ((lhs != vertex) && (rhs != vertex))
            {
                continue;
            }
            const auto &next = (lhs != vertex) ? lhs : rhs;
            if ((next == "start") || (next == vertex) || (next.empty()))
            {
                continue;
            }
            const auto isSmall = [](const auto &c) -> bool {
                return ((c.size() > 0) && (c[0] >= 'a') && (c[0] <= 'z'));
            };
            if (isSmall(next))
            {

                bool alreadyVisited = (prevPath.end() != std::ranges::find(prevPath, next));
                if (alreadyVisited)
                {
                    if (aSingleSmallOnceMayBeVisitedTwice)
                    {
                        const bool hasAnySmallOneBeVisitedTwice =
                            std::ranges::any_of(prevPath, [&prevPath, &isSmall](const auto &cave) {
                                return (isSmall(cave) && (std::ranges::count(prevPath, cave) > 1));
                            });
                        if (hasAnySmallOneBeVisitedTwice)
                        {
                            continue;
                        }
                    }
                    else
                    {
                        continue;
                    }
                }
            }
            auto extendedPath = prevPath;
            extendedPath.push_back(next);

            if (next == "end")
            {
                pathsCnt += 1;
            }
            else
            {
                const auto childPathCnt = recursion(extendedPath, recursion);
                if (childPathCnt < 0)
                {
                    return childPathCnt;
                }
                pathsCnt += childPathCnt;
            }
        }
        return pathsCnt;
    };
    return countPaths(path, countPaths);
}

#if AOC_Y2021_CONSTEXPR_UNIT_TEST
namespace
{
[[maybe_unused]] consteval void Test()
{
    constexpr std::array shortExample = {
        std::make_pair<std::string_view, std::string_view>("start", "A"),
        std::make_pair<std::string_view, std::string_view>("start", "b"),
        std::make_pair<std::string_view, std::string_view>("A", "c"),
        std::make_pair<std::string_view, std::string_view>("A", "b"),
        std::make_pair<std::string_view, std::string_view>("b", "d"),
        std::make_pair<std::string_view, std::string_view>("A", "end"),
        std::make_pair<std::string_view, std::string_view>("b", "end"),
    };

    constexpr std::array mediumExample = {
        std::make_pair<std::string_view, std::string_view>("dc", "end"),
        std::make_pair<std::string_view, std::string_view>("HN", "start"),
        std::make_pair<std::string_view, std::string_view>("start", "kj"),
        std::make_pair<std::string_view, std::string_view>("dc", "start"),
        std::make_pair<std::string_view, std::string_view>("dc", "HN"),
        std::make_pair<std::string_view, std::string_view>("LN", "dc"),
        std::make_pair<std::string_view, std::string_view>("HN", "end"),
        std::make_pair<std::string_view, std::string_view>("kj", "sa"),
        std::make_pair<std::string_view, std::string_view>("kj", "HN"),
        std::make_pair<std::string_view, std::string_view>("kj", "dc"),
    };

    constexpr std::array largeExample = {
        std::make_pair<std::string_view, std::string_view>("fs", "end"),
        std::make_pair<std::string_view, std::string_view>("he", "DX"),
        std::make_pair<std::string_view, std::string_view>("fs", "he"),
        std::make_pair<std::string_view, std::string_view>("start", "DX"),
        std::make_pair<std::string_view, std::string_view>("pj", "DX"),
        std::make_pair<std::string_view, std::string_view>("end", "zg"),
        std::make_pair<std::string_view, std::string_view>("zg", "sl"),
        std::make_pair<std::string_view, std::string_view>("zg", "pj"),
        std::make_pair<std::string_view, std::string_view>("pj", "he"),
        std::make_pair<std::string_view, std::string_view>("RW", "he"),
        std::make_pair<std::string_view, std::string_view>("fs", "DX"),
        std::make_pair<std::string_view, std::string_view>("pj", "RW"),
        std::make_pair<std::string_view, std::string_view>("zg", "RW"),
        std::make_pair<std::string_view, std::string_view>("start", "pj"),
        std::make_pair<std::string_view, std::string_view>("he", "WI"),
        std::make_pair<std::string_view, std::string_view>("zg", "he"),
        std::make_pair<std::string_view, std::string_view>("pj", "fs"),
        std::make_pair<std::string_view, std::string_view>("start", "RW"),
    };

    static_assert(CountPaths(shortExample, false) == 10, "Day 12 part 1: error calculating result for small example");
    static_assert(CountPaths(mediumExample, false) == 19, "Day 12 part 1: error calculating result for medium example");
    static_assert(CountPaths(largeExample, false) == 226, "Day 12 part 1: error calculating result for large example");
    static_assert(CountPaths(shortExample, true) == 36, "Day 12 part 2: error calculating result for small example");
    static_assert(CountPaths(mediumExample, true) == 103, "Day 12 part 2: error calculating result for medium example");
    static_assert(CountPaths(largeExample, true) == 3509, "Day 12 part 2: error calculating result for large example");
}
} // namespace
#endif // AOC_Y2021_CONSTEXPR_UNIT_TEST

} // namespace

} // namespace Day12

AOC_Y2021_PUZZLE_CLASS_DECLARATION(12)

class PuzzleDay12Impl final
{
  public:
    PuzzleDay12Impl(const std::string_view &input) : graph(Day12::ParseInput(input))
    {
    }
    const auto &GetEdges() const
    {
        return graph.edges;
    }

  private:
    Day12::Graph graph;
};

PuzzleDay12::PuzzleDay12(const std::string_view input)
    : pImpl(std::make_unique<PuzzleDay12Impl>(input))
{
}
PuzzleDay12::~PuzzleDay12() = default;

[[nodiscard]] IPuzzle::Solution_t PuzzleDay12::Part1()
{
    if (!pImpl)
    {
        return std::monostate{};
    }
    const auto result = Day12::CountPaths(pImpl->GetEdges(), false);
    if (result < 0)
    {
        return std::monostate{};
    }
    else
    {
        return result;
    }
}

[[nodiscard]] IPuzzle::Solution_t PuzzleDay12::Part2()
{
    if (!pImpl)
    {
        return std::monostate{};
    }
    const auto result = Day12::CountPaths(pImpl->GetEdges(), true);
    if (result < 0)
    {
        return std::monostate{};
    }
    else
    {
        return result;
    }
}

} // namespace AOC::Y2021
