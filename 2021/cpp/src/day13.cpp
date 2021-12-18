#include "ipuzzle.hpp"
#include "puzzle_common.hpp"
#include "utils.hpp"

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <memory>
#include <numeric>
#include <ranges>
#include <string>
#include <string_view>
#include <utility>
#include <variant>
#include <vector>

namespace AOC::Y2021
{

namespace Day13
{

namespace
{

using Coord_t = std::uint16_t;
using Instructions_t = std::vector<std::pair<bool, Coord_t>>;

struct DotMap
{
    AOC_Y2021_CONSTEXPR DotMap(){};
    [[nodiscard]] AOC_Y2021_CONSTEXPR bool Contains(const std::pair<Coord_t, Coord_t> &dot)
    {
        return std::ranges::any_of(dots, [&dot](const auto &d) { return d == dot; });
    }
    AOC_Y2021_CONSTEXPR void Insert(const std::pair<Coord_t, Coord_t> &dot)
    {
        if (!Contains(dot))
        {
            dots.push_back(dot);
        }
    }
    AOC_Y2021_CONSTEXPR auto CountDots() const
    {
        return dots.size();
    }

    const auto &Get() const
    {
        return dots;
    }

  public:
    std::vector<std::pair<Coord_t, Coord_t>> dots;
};

struct TransparentPaper
{
    DotMap dots;
    Instructions_t instructions;
};

[[nodiscard]] TransparentPaper ParseInput(const std::string_view &input)
{
    TransparentPaper parsedPaper;
    bool foundBeginOfInstructionBlock = false;
    auto forEachLine = [&parsedPaper, &foundBeginOfInstructionBlock](const std::string_view &line) -> bool {
        if (line.empty())
        {
            foundBeginOfInstructionBlock = true;
            return true;
        }
        if (foundBeginOfInstructionBlock)
        {
            constexpr std::string_view instrFoldX = "fold along x=";
            constexpr std::string_view instrFoldY = "fold along y=";
            static_assert(instrFoldX.size() == instrFoldY.size(), "");
            const bool bFoldX = line.starts_with(instrFoldX);
            if (bFoldX || line.starts_with(instrFoldY))
            {
                parsedPaper.instructions.emplace_back(bFoldX, ParseNumber<Coord_t>(line.substr(instrFoldX.size())));
                return true;
            }
            else
            {
                return false;
            }
        }
        else
        {
            constexpr char delimiter = ',';
            const auto delimiterPos = line.find(delimiter);
            if ((delimiterPos == 0) || (delimiterPos == line.npos) || ((delimiterPos + 1u) == line.size()))
            {
                return false;
            }
            parsedPaper.dots.Insert({ParseNumber<Coord_t>(line), ParseNumber<Coord_t>(line.substr(delimiterPos + 1u))});
            return true;
        }
    };
    if (!ParseLines(input, forEachLine, false))
    {
        return {};
    }
    else
    {
        return parsedPaper;
    }
}

AOC_Y2021_CONSTEXPR bool Fold(auto &dotsOnPaper, const auto &instructions,
                              std::size_t foldingIterations = 0 /* 0: no limit */)
{
    if ((instructions.size() < foldingIterations) || (dotsOnPaper.CountDots() < 1))
    {
        return false;
    }

    for (const auto &[bFoldLeft, line] : instructions)
    {
        DotMap foldedPaper;
        for (const auto &oldDot : dotsOnPaper.dots)
        {
            const auto oldCoordinate = (bFoldLeft) ? oldDot.first : oldDot.second;

            if (oldCoordinate < line)
            {
                foldedPaper.Insert(oldDot);
            }
            else
            {
                if (oldCoordinate > (2 * line))
                {
                    return false;
                }
                const Coord_t foldedCoordinate = ((2 * line) - oldCoordinate);
                if (bFoldLeft)
                {
                    foldedPaper.Insert({foldedCoordinate, oldDot.second});
                }
                else
                {
                    foldedPaper.Insert({oldDot.first, foldedCoordinate});
                }
            }
        }
        dotsOnPaper = std::move(foldedPaper);
        if (foldingIterations > 0)
        {
            if ((--foldingIterations) == 0)
            {
                break;
            }
        }
    }

    return true;
}

[[nodiscard]] AOC_Y2021_CONSTEXPR std::int64_t CountDotsAfterOneInstruction(const auto &dotsAndInstructions)
{
    DotMap mutableDotmap = dotsAndInstructions.dots;
    const std::size_t foldingIterations = 1u;
    if (!Fold(mutableDotmap, dotsAndInstructions.instructions, foldingIterations))
    {
        return -__LINE__;
    }
    return mutableDotmap.CountDots();
}

[[nodiscard]] AOC_Y2021_CONSTEXPR std::string RestoreCodeByFolding(const auto &dotsAndInstructions)
{
    DotMap mutableDotmap = dotsAndInstructions.dots;
    if (!Fold(mutableDotmap, dotsAndInstructions.instructions))
    {
        return "";
    }
    if (mutableDotmap.Get().size() < 1)
    {
        return "";
    }
    const auto maxX = std::ranges::max_element(mutableDotmap.Get(), [](const auto &lhs, const auto &rhs) {
                          return (lhs.first < rhs.first);
                      })->first;
    const auto maxY = std::ranges::max_element(mutableDotmap.Get(), [](const auto &lhs, const auto &rhs) {
                          return (lhs.second < rhs.second);
                      })->second;
    if ((maxX == 0) || (maxY == 0))
    {
        return "";
    }
    constexpr char lf = '\n';
    const std::size_t lineWidth = maxX + 1u + sizeof(lf);
    const std::size_t visualizationSize = lineWidth * (maxY + 1u);
    std::string visualization(visualizationSize, '.');
    for (size_t i = maxX + 1u; i < visualizationSize; i += lineWidth)
    {
        visualization[i] = lf;
    }
    for (const auto &[x, y] : mutableDotmap.Get())
    {
        visualization[(y * lineWidth) + x] = '#';
    }

    return visualization;
}

} // namespace

} // namespace Day13

AOC_Y2021_PUZZLE_CLASS_DECLARATION(13)

class PuzzleDay13Impl final
{
  public:
    PuzzleDay13Impl(const std::string_view &input) : transparentPaper(Day13::ParseInput(input))
    {
    }
    const auto &GetTransParentPaper() const
    {
        return transparentPaper;
    }

  private:
    Day13::TransparentPaper transparentPaper;
};

PuzzleDay13::PuzzleDay13(const std::string_view input) : pImpl(std::make_unique<PuzzleDay13Impl>(input))
{
}
PuzzleDay13::~PuzzleDay13() = default;

[[nodiscard]] IPuzzle::Solution_t PuzzleDay13::Part1()
{
    if (!pImpl)
    {
        return std::monostate{};
    }
    const auto result = Day13::CountDotsAfterOneInstruction(pImpl->GetTransParentPaper());
    if (result < 0)
    {
        return std::monostate{};
    }
    else
    {
        return result;
    }
}

[[nodiscard]] IPuzzle::Solution_t PuzzleDay13::Part2()
{
    if (!pImpl)
    {
        return std::monostate{};
    }
    return Day13::RestoreCodeByFolding(pImpl->GetTransParentPaper());
}

} // namespace AOC::Y2021
