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
#include <vector>

namespace AOC::Y2021
{

namespace
{
class BingoGame
{
  private:
    class Board
    {
      public:
        constexpr static auto numLines = 5;
        constexpr static auto numsPerLine = 5;
        struct Number
        {
            std::uint8_t num : 7u;
            std::uint8_t drawn : 1u;
        };
        static_assert(sizeof(Number) == 1, "");

      public:
        AOC_Y2021_CONSTEXPR std::optional<uint32_t> MarkNumber(const uint8_t number)
        {
            for (auto &row : aaNums)
            {
                for (auto &num : row)
                {
                    if (num.num == number)
                    {
                        num.drawn = 1;
                    }
                }
            }
            auto fnIsHorizontalBingo = [this]() -> bool {
                return std::ranges::any_of(aaNums, [](const auto &row) -> bool {
                    return std::ranges::all_of(row, [](const auto &n) { return n.drawn != 0; });
                });
            };
            auto fnVerticalRowBingo = [this]() -> bool {
                for (size_t col = 0; col < aaNums[0u].size(); col++)
                {
                    bool bIsColBingo = true;
                    for (size_t row = 0u; row != aaNums.size(); row++)
                    {
                        if (aaNums[row][col].drawn == 0)
                        {
                            bIsColBingo = false;
                            break;
                        }
                    }
                    if (bIsColBingo)
                    {
                        return true;
                    }
                }
                return false;
            };
            if (fnIsHorizontalBingo() || fnVerticalRowBingo())
            {
                std::uint32_t sumOfUnmarkedNums = 0;
                for (const auto &row : aaNums)
                {
                    for (const auto &num : row)
                    {
                        if (!num.drawn)
                        {
                            sumOfUnmarkedNums += num.num;
                        }
                    }
                }
                const auto score = sumOfUnmarkedNums * number;
                return score;
            }
            else
            {
                return std::nullopt;
            }
        }

      public:
        std::array<std::array<Number, numsPerLine>, numLines> aaNums{};
    };

  public:
    AOC_Y2021_CONSTEXPR BingoGame(std::string_view input)
    {
        const auto endOfFirstLine = input.find_first_of('\n');
        if (endOfFirstLine == input.npos)
        {
            return;
        }
        auto firstLine = input.substr(0, endOfFirstLine);
        input.remove_prefix(endOfFirstLine);
        while (firstLine.size())
        {
            drawnNumbers.push_back(ParseNumber<std::uint8_t>(firstLine));
            const auto delimiterPos = firstLine.find_first_of(',');
            if (delimiterPos == firstLine.npos)
            {
                break;
            }
            firstLine.remove_prefix(delimiterPos + 1);
        }
        std::ranges::reverse(drawnNumbers);

        constexpr auto whitespaceChars = "\t\r\n ";
        while (input.size() > 5 && (input.find_first_not_of(whitespaceChars) != input.npos))
        {
            auto &board = boards.emplace_back();
            for (size_t row = 0; row != Board::numLines; ++row)
            {
                for (size_t col = 0; col != Board::numsPerLine; ++col)
                {
                    const auto start = input.find_first_not_of(whitespaceChars);
                    if (start == input.npos)
                    {
                        input.remove_suffix(input.size());
                        break;
                    }
                    input.remove_prefix(start);
                    const auto num = ParseNumber<std::uint8_t>(input);
                    board.aaNums[row][col].num = num;
                    const auto end = input.find_first_of(whitespaceChars);
                    if (end == input.npos)
                    {
                        input.remove_suffix(input.size());
                        break;
                    }
                    input.remove_prefix(end);
                }
            }
        }
        return;
    }

    AOC_Y2021_CONSTEXPR std::optional<int64_t> DrawNextNumber()
    {
        if (!drawnNumbers.size())
        {
            return std::nullopt;
        }
        std::optional<int64_t> score = std::nullopt;
        const auto drawn = drawnNumbers.back();
        drawnNumbers.pop_back();
        for (auto it = boards.begin(); it != boards.end();)
        {
            const auto score_ = it->MarkNumber(drawn);
            bool bHasBingo = score_.has_value();
            // std::optional<int64_t> score_;
            if (bHasBingo)
            {
                if (!score.has_value())
                {
                    score = score_;
                }
                it = boards.erase(it);
            }
            else
            {
                it++;
            }
        }
        return score;
    }

    AOC_Y2021_CONSTEXPR std::optional<int64_t> PlayPart1()
    {
        while (boards.size() && drawnNumbers.size())
        {
            const auto score = DrawNextNumber();
            if (score.has_value())
            {
                return score;
            }
        }
        return std::nullopt;
    }

    AOC_Y2021_CONSTEXPR std::optional<int64_t> PlayPart2()
    {
        std::optional<int64_t> score = std::nullopt;
        while (boards.size() && drawnNumbers.size())
        {
            const auto score_ = DrawNextNumber();
            if (score_.has_value())
            {
                score = score_;
            }
        }
        return score;
    }

  private:
    std::vector<std::uint8_t> drawnNumbers;
    std::vector<Board> boards;
};

} // namespace

class PuzzleDay04Impl final
{

  public:
    AOC_Y2021_CONSTEXPR PuzzleDay04Impl(std::string_view input) : game(input)
    {
    }

  public:
    BingoGame game;
};
AOC_Y2021_PUZZLE_CLASS_DECLARATION(04)

PuzzleDay04::PuzzleDay04(const std::string_view input) : pImpl(std::make_unique<PuzzleDay04Impl>(input))
{
}

PuzzleDay04::~PuzzleDay04() = default;

std::optional<std::int64_t> PuzzleDay04::Part1()
{
    if (!pImpl)
    {
        return std::nullopt;
    }
    return pImpl->game.PlayPart1();
}

std::optional<std::int64_t> PuzzleDay04::Part2()
{
    if (!pImpl)
    {
        return std::nullopt;
    }
    return pImpl->game.PlayPart2();
}

#if AOC_Y2021_CONSTEXPR_UNIT_TEST
namespace
{

[[maybe_unused]] consteval void TestDay04()
{
    constexpr auto exampleInput =
        "7,4,9,5,11,17,23,2,0,14,21,24,10,16,13,6,15,25,12,22,18,20,8,19,3,26,1\n\r\n22 13 17 11  0\r\n 8  2 23  4 "
        "24\r\n21  9 14 16  7\r\n 6 10  3 18  5\r\n 1 12 20 15 19\n\r\n 3 15  0  2 22\r\n 9 18 13 17  5\n19  8  7 25 "
        "23\r\n20 11 10 24  4\n14 21 16 12  6\r\n\n14 21 17 24  4\r\n10 16 15  9 19\r\n18  8 23 26 20\r\n22 11 13  6  "
        "5\r\n 2  0 12  3  7\r";

    static_assert(BingoGame(exampleInput).PlayPart1().has_value(), "Day 4 Part 1 failed to calculate a score");
    static_assert(BingoGame(exampleInput).PlayPart1().value() == 4512, "Day 4 Part 1 failed: wrong score");
    static_assert(BingoGame(exampleInput).PlayPart2().has_value(), "Day 4 Part 2 failed to calculate a score");
    static_assert(BingoGame(exampleInput).PlayPart2().value() == 1924, "Day 4 Part 2 failed: wrong score");
}

} // namespace
#endif // AOC_Y2021_CONSTEXPR_UNIT_TEST
} // namespace AOC::Y2021
