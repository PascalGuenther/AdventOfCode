#include "ipuzzle.hpp"
#include "puzzle_common.hpp"
#include "utils.hpp"

#include <algorithm>
#include <array>
#include <cstddef>
#include <cstdint>
#include <memory>
#include <numeric>
#include <optional>
#include <ranges>
#include <string_view>
#include <utility>
#include <vector>

namespace AOC::Y2021
{

namespace Day14
{

namespace
{

struct PairInsertionRule
{
    std::array<char, 2> from;
    char to;
};

struct ParserOutput
{
    std::string rawInput;
    std::string_view polyTpl;
    std::vector<PairInsertionRule> pairInsertionRules;
};

[[nodiscard]] AOC_Y2021_CONSTEXPR ParserOutput ParseInput(const std::string_view &input)
{
    ParserOutput parsed = {
        .rawInput{std::string{input}},
        .polyTpl{},
        .pairInsertionRules{},
    };
    bool foundBeginOfInsertionRules = false;
    auto forEachLine = [&parsed, &foundBeginOfInsertionRules](const std::string_view &line) -> bool {
        if (line.empty())
        {
            if (foundBeginOfInsertionRules)
            {
                return false;
            }
            else
            {

                foundBeginOfInsertionRules = true;
                return true;
            }
        }
        if (foundBeginOfInsertionRules)
        {
            constexpr std::string_view delimiter = " -> ";
            const std::size_t expectedLineSize =
                sizeof(PairInsertionRule::from) + delimiter.size() + sizeof(PairInsertionRule::to);
            if ((line.size() != expectedLineSize) || (line.find(delimiter) != sizeof(PairInsertionRule::from)))
            {
                return false;
            }
            parsed.pairInsertionRules.push_back({
                .from =
                    {
                        line[0],
                        line[1],
                    },
                .to = line[sizeof(PairInsertionRule::from) + delimiter.size()],
            });
            return true;
        }
        else
        {
            parsed.polyTpl = line;
            return true;
        }
    };
    if (!ParseLines(parsed.rawInput, forEachLine, false))
    {
        return {};
    }
    else
    {
        return parsed;
    }
}

AOC_Y2021_CONSTEXPR IPuzzle::Solution_t GrowPolymer(const ParserOutput &input, const std::uint8_t steps)
{
    const auto &polyTpl = input.polyTpl;
    const auto &rules = input.pairInsertionRules;
    if (polyTpl.empty() || rules.empty())
    {
        return std::monostate{};
    }
    if (std::ranges::any_of(polyTpl, [](const auto &c) { return ((c < 'A') || (c > 'Z')); }))
    {
        return std::monostate{};
    }
    if (std::ranges::any_of(rules, [](const auto &rule) { return ((rule.to < 'A') || (rule.to > 'Z')); }))
    {
        return std::monostate{};
    }

    using Frequencies_t = std::array<std::int64_t, 'Z' - 'A' + 1>;
    Frequencies_t frequencies{};
    for (const auto &c : polyTpl)
    {
        frequencies[static_cast<unsigned char>(c - 'A')] += 1;
    }
    std::vector<std::optional<Frequencies_t>> recursionCache((steps + 1) * rules.size());
    static_assert(frequencies.size() == 26);
    if (steps > 1)
    {
        for (size_t i = 0; i != (polyTpl.size() - 1); ++i)
        {
            const auto processSteps = [&rules, &recursionCache](const std::size_t index, Frequencies_t &accFreq,
                                                                std::uint8_t remainingSteps, auto &&recursion) -> bool {
                if (remainingSteps == 0)
                {
                    return true;
                }
                const auto cacheIndex = (remainingSteps * rules.size()) + index;
                if (!recursionCache[cacheIndex].has_value())
                {
                    const std::array<char, 2> newPairs[] = {
                        std::array<char, 2>{
                            rules[index].from[0],
                            rules[index].to,
                        },
                        std::array<char, 2>{
                            rules[index].to,
                            rules[index].from[1],
                        },
                    };
                    Frequencies_t ourFreq{};
                    ourFreq[static_cast<unsigned char>(rules[index].to - 'A')] = 1;
                    for (const auto &newPair : newPairs)
                    {
                        const auto pairIt = std::ranges::find_if(
                            rules, [&newPair](const auto &rule) { return (rule.from == newPair); });
                        if (pairIt == rules.end())
                        {
                            return false;
                        }
                        const auto newPairIndex = static_cast<std::size_t>(std::distance(rules.begin(), pairIt));

                        if (!recursion(newPairIndex, ourFreq, remainingSteps - 1, recursion))
                        {
                            return false;
                        }
                    }
                    recursionCache[cacheIndex] = ourFreq;
                }

                const auto &cachedValue = recursionCache[cacheIndex].value();
                std::transform(accFreq.begin(), accFreq.end(), cachedValue.begin(), accFreq.begin(),
                               std::plus<std::int64_t>());
                return true;
            };
            const std::array<char, 2> localPair = {polyTpl[i], polyTpl[i + 1u]};
            const auto localPairIt =
                std::ranges::find_if(rules, [&localPair](const auto &rule) { return (rule.from == localPair); });
            if (localPairIt == rules.end())
            {
                return false;
            }
            const auto localPairIndex = static_cast<std::size_t>(std::distance(rules.begin(), localPairIt));
            if (!processSteps(localPairIndex, frequencies, steps, processSteps))
            {
                return std::monostate{};
            }
        }
    }

    std::int64_t minFreq = INT64_MAX;
    std::int64_t maxFreq = 0;
    for (const auto &f : frequencies)
    {
        if ((f > 0) && (f < minFreq))
        {
            minFreq = f;
        }
        if (f > maxFreq)
        {
            maxFreq = f;
        }
    }
    return std::int64_t{maxFreq - minFreq};

} // namespace

#if AOC_Y2021_CONSTEXPR_UNIT_TEST
namespace
{
[[maybe_unused]] consteval void Test()
{
    constexpr std::string_view exampleInput = "NNCB\n\nCH -> B\nHH -> N\nCB -> H\nNH -> C\nHB -> C\nHC -> B\r\nHN -> "
                                              "C\nNN -> C\nBH -> H\nNC -> B\nNB -> B\nBN "
                                              "-> B\r\nBB -> N\nBC -> B\nCC -> N\nCN -> C";
    static_assert(std::get<std::int64_t>(GrowPolymer(ParseInput(exampleInput), 10)) == 1588,
                  "Day 14 part 1: error calculating resut");
    static_assert(std::get<std::int64_t>(GrowPolymer(ParseInput(exampleInput), 40)) == 2'188'189'693'529,
                  "Day 14 part 2: error calculating resut");
}

} // namespace
#endif // AOC_Y2021_CONSTEXPR_UNIT_TEST

} // namespace

} // namespace Day14

AOC_Y2021_PUZZLE_CLASS_DECLARATION(14)

class PuzzleDay14Impl final
{
  public:
    PuzzleDay14Impl(const std::string_view &input) : parsed(Day14::ParseInput(input))
    {
    }

    const auto &Get() const
    {
        return parsed;
    }

  private:
    Day14::ParserOutput parsed;
};

PuzzleDay14::PuzzleDay14(const std::string_view input) : pImpl(std::make_unique<PuzzleDay14Impl>(input))
{
}
PuzzleDay14::~PuzzleDay14() = default;

[[nodiscard]] IPuzzle::Solution_t PuzzleDay14::Part1()
{
    if (!pImpl)
    {
        return std::monostate{};
    }
    return GrowPolymer(pImpl->Get(), 10);
}

[[nodiscard]] IPuzzle::Solution_t PuzzleDay14::Part2()
{
    if (!pImpl)
    {
        return std::monostate{};
    }
    return GrowPolymer(pImpl->Get(), 40);
}

} // namespace AOC::Y2021
