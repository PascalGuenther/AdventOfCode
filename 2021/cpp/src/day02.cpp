#include "ipuzzle.hpp"
#include "puzzle_common.hpp"
#include "utils.hpp"

#include <memory>
#include <numeric>
#include <string_view>
#include <vector>

namespace AOC::Y2021
{

class PuzzleDay02Impl final
{
  public:
    AOC_Y2021_CONSTEXPR PuzzleDay02Impl(std::string_view writtenInstructions)
    {
        auto inputSize = writtenInstructions.size();
        while (inputSize > 4u)
        {
            const auto valuePos = writtenInstructions.find(' ');
            const auto value = ParseNumber<int>(writtenInstructions.substr(valuePos));
            if (writtenInstructions.starts_with("forward"))
            {
                instructions.push_back({.forward = value, .down = 0});
            }
            else if (writtenInstructions.starts_with("up"))
            {
                instructions.push_back({.forward = 0, .down = -value});
            }
            else if (writtenInstructions.starts_with("down"))
            {
                instructions.push_back({.forward = 0, .down = value});
            }
            else
            {
                break;
            }

            const auto endOfLine = writtenInstructions.find('\n');
            if (endOfLine == writtenInstructions.npos)
            {
                break;
            }
            const auto startOfNextLine = endOfLine + 1u;
            if (startOfNextLine >= inputSize)
            {
                break;
            }
            writtenInstructions.remove_prefix(startOfNextLine);
            inputSize = writtenInstructions.size();
        }
    }

    AOC_Y2021_CONSTEXPR std::int64_t Part01()
    {
        const Position startPos{0, 0};
        const auto endPos =
            std::accumulate(instructions.begin(), instructions.end(), startPos, [](Position sum, const auto &other) {
                return Position{sum.horizontal + other.forward, sum.depth + other.down};
            });
        const auto result = endPos.depth * endPos.horizontal;
        return result;
    }

    AOC_Y2021_CONSTEXPR std::int64_t Part02()
    {
        int aim = 0;
        Position pos{.horizontal = 0, .depth = 0};
        for (const auto &instruction : instructions)
        {
            if (instruction.forward != 0)
            {
                pos.horizontal += instruction.forward;
                pos.depth += aim * instruction.forward;
            }
            else
            {
                aim += instruction.down;
            }
        }
        const auto result = pos.depth * pos.horizontal;
        return result;
    }

    auto GetDataSize() const
    {
        return instructions.size();
    }

  private:
    struct Position
    {
        int horizontal;
        int depth;
    };
    struct Instruction
    {
        int forward;
        int down;
    };

  private:
    std::vector<Instruction> instructions;
};

AOC_Y2021_PUZZLE_CLASS_DECLARATION(02)

PuzzleDay02::PuzzleDay02(const std::string_view input) : pImpl(std::make_unique<PuzzleDay02Impl>(input))
{
}

PuzzleDay02::~PuzzleDay02() = default;

[[nodiscard]] IPuzzle::Solution_t PuzzleDay02::Part1()
{
    if (!pImpl || (pImpl->GetDataSize() < 2))
    {
        return std::monostate{};
    }
    return pImpl->Part01();
}

[[nodiscard]] IPuzzle::Solution_t PuzzleDay02::Part2()
{
    if (!pImpl || (pImpl->GetDataSize() <= 2))
    {
        return std::monostate{};
    }
    return pImpl->Part02();
}

#if AOC_Y2021_CONSTEXPR_UNIT_TEST
namespace
{
constexpr std::string_view exampleInput = "forward 5\ndown 5\r\nforward 8\nup 3\ndown 8\nforward 2\n";

consteval bool TestDay02Part1()
{

    PuzzleDay02Impl impl{exampleInput};

    return (150 == impl.Part01());
}
static_assert(TestDay02Part1(), "Day 02 Part 1 failed");
consteval bool TestDay02Part2()
{

    PuzzleDay02Impl impl{exampleInput};

    return (900 == impl.Part02());
}
static_assert(TestDay02Part2(), "Day 02 Part 2 failed");

} // namespace
#endif // AOC_Y2021_CONSTEXPR_UNIT_TEST
} // namespace AOC::Y2021
