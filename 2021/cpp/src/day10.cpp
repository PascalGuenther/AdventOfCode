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

namespace AOC::Y2021
{

namespace Day10
{

namespace
{

AOC_Y2021_CONSTEXPR std::int64_t CalculateScore(std::string_view input, const bool runAutoComplete)
{
    struct Symbol
    {
        char open;
        char close;
        std::int64_t errorPoints;
        std::int64_t autoCompletePoints;
    };
    constexpr std::array<Symbol, 4> aSymbols{{
        {
            .open = '(',
            .close = ')',
            .errorPoints = 3,
            .autoCompletePoints = 1,
        },
        {
            .open = '[',
            .close = ']',
            .errorPoints = 57,
            .autoCompletePoints = 2,
        },
        {
            .open = '{',
            .close = '}',
            .errorPoints = 1197,
            .autoCompletePoints = 3,
        },
        {
            .open = '<',
            .close = '>',
            .errorPoints = 25137,
            .autoCompletePoints = 4,
        },
    }};
    std::vector<std::int64_t> scores;
    if (!runAutoComplete)
    {
        scores.push_back(0);
    }
    std::vector<decltype(aSymbols.begin())> stack;
    while (input.size())
    {

        const auto endOfLine = input.find_first_of("\r\n");
        const auto line = (endOfLine == input.npos) ? input : input.substr(0, endOfLine);

        stack.clear();
        stack.reserve(line.size());
        std::int64_t lineErrorPoints = 0u;
        for (const auto &c : line)
        {
            const auto it =
                std::find_if(aSymbols.begin(), aSymbols.end(), [&c](const auto &s) { return (s.open == c); });
            const bool bOpensAChunk = (it != aSymbols.end());
            if (bOpensAChunk)
            {
                stack.push_back(it);
            }
            else
            {
                const auto it =
                    std::find_if(aSymbols.begin(), aSymbols.end(), [&c](const auto &s) { return (s.close == c); });
                const bool bIsClosingSymbol = (it != aSymbols.end());
                if (!bIsClosingSymbol)
                {
                    return -__LINE__; // error parsing input
                }
                const bool bIsExpectedClosingChar = ((stack.size() > 0) && (stack.back() == it));
                if (bIsExpectedClosingChar)
                {
                    stack.pop_back();
                }
                else
                {
                    lineErrorPoints = it->errorPoints;
                    break;
                }
            }
        }

        if (!runAutoComplete)
        {
            scores[0] += lineErrorPoints;
        }
        else
        {
            if (lineErrorPoints == 0)
            {

                scores.push_back(0);
                for (auto itMustClose = stack.rbegin(); itMustClose != stack.rend(); ++itMustClose)
                {
                    scores.back() *= 5;
                    scores.back() += (*itMustClose)->autoCompletePoints;
                }
            }
        }

        const auto lf = input.find('\n');
        if (lf == input.npos)
        {
            break;
        }
        input.remove_prefix(lf + 1);
    }
    if (!runAutoComplete)
    {
        return scores[0];
    }
    else
    {
        const auto middle = scores.begin() + (scores.size() / 2);
        if (middle >= scores.end())
        {
            return -__LINE__;
        }
        std::nth_element(scores.begin(), middle, scores.end());
        return (*middle);
    }
}

#if AOC_Y2021_CONSTEXPR_UNIT_TEST
namespace
{
[[maybe_unused]] consteval void Test()
{
    constexpr std::string_view exampleInput =
        "[({(<(())[]>[[{[]{<()<>>\r\n[(()[<>])]({[<{<<[]>>(\r\n{([(<{}[<>[]}>{[]{[(<()>\n(((({<>}<{<{<>}{[]{[]{}\r\n["
        "[<[([]))<([[{}[[()]]]\r\n[{[{({}]{}}([{[{{{}}([]\r\n{<[[]]>}<{[{[{[]{()[[[]\r\n[<(<(<(<{}))><([]([]()\r\n<{([("
        "[[(<>()){}]>(<<{{\r\n<{([{{}}[<[[[<>{}]]]>[]]";
    static_assert(CalculateScore(exampleInput, false) == 26'397, "Day 10 part 1: error calculating syntax error score");
    static_assert(CalculateScore(exampleInput, true) == 288'957,
                  "Day 10 part 2: error calculating median autocomplete score");
} // namespace
#endif // AOC_Y2021_CONSTEXPR_UNIT_TEST

} // namespace

} // namespace Day10

class PuzzleDay10Impl final
{

  public:
    AOC_Y2021_CONSTEXPR PuzzleDay10Impl(const std::string_view &input) : input(input)
    {
    }

  public:
    std::string input;
};

AOC_Y2021_PUZZLE_CLASS_DECLARATION(10)

PuzzleDay10::PuzzleDay10(const std::string_view input) : pImpl(std::make_unique<PuzzleDay10Impl>(input))
{
}
PuzzleDay10::~PuzzleDay10() = default;

[[nodiscard]] IPuzzle::Solution_t PuzzleDay10::Part1()
{
    if (!pImpl || pImpl->input.empty())
    {
        return std::monostate{};
    }
    const auto result = Day10::CalculateScore(pImpl->input, false);
    if (result < 0)
    {
        return std::monostate{};
    }
    else
    {
        return result;
    }
}

[[nodiscard]] IPuzzle::Solution_t PuzzleDay10::Part2()
{
    if (!pImpl || pImpl->input.empty())
    {
        return std::monostate{};
    }
    const auto result = Day10::CalculateScore(pImpl->input, true);
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
