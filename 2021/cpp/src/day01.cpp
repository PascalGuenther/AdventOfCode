#include "ipuzzle.hpp"
#include "puzzle_common.hpp"
#include "utils.hpp"

#include <algorithm>
#include <memory>
#include <numeric>
#include <string_view>
#include <utility>
#include <vector>

namespace AOC::Y2021
{

constexpr std::size_t windowSize = 3u;

class PuzzleDay01Impl final
{
  public:
    AOC_Y2021_CONSTEXPR PuzzleDay01Impl(std::vector<int> &&measurements) : measurements(measurements)
    {
    }

    AOC_Y2021_CONSTEXPR std::int64_t Part01()
    {
        if (measurements.size() < 2u)
        {
            return 0u;
        }
        std::vector<int> adjacent_diffs(measurements.size(), -1);
        std::adjacent_difference(measurements.begin(), measurements.end(), adjacent_diffs.begin());
        return std::count_if(adjacent_diffs.begin() + 1u, adjacent_diffs.end(), [](const auto &i) { return i > 0; });
    }

    AOC_Y2021_CONSTEXPR std::int64_t Part02()
    {
        if (measurements.size() <= windowSize)
        {
            return 0u;
        }
        const auto vecEnd = measurements.end();
        int prevWindowSum = 0;
        std::int64_t ret = -1;
        for (auto end = measurements.begin() + windowSize; end <= vecEnd; end++)
        {
            const int windowSum = std::accumulate(end - windowSize, end, 0u);
            if (windowSum > prevWindowSum)
            {
                ret++;
            }
            prevWindowSum = windowSum;
        }
        return ret > 0 ? ret : 0;
    }

    auto GetDataSize() const
    {
        return measurements.size();
    }

  private:
    std::vector<int> measurements;
};

AOC_Y2021_PUZZLE_CLASS_DECLARATION(01)

PuzzleDay01::PuzzleDay01(const std::string_view input)
    : pImpl(std::make_unique<PuzzleDay01Impl>(ParseToVectorOfNums<int, '\n'>(input)))
{
}

PuzzleDay01::~PuzzleDay01() = default;

[[nodiscard]] IPuzzle::Solution_t PuzzleDay01::Part1()
{
    if (!pImpl || (pImpl->GetDataSize() < 2))
    {
        return std::monostate{};
    }
    return pImpl->Part01();
}

[[nodiscard]] IPuzzle::Solution_t PuzzleDay01::Part2()
{
    if (!pImpl || (pImpl->GetDataSize() <= windowSize))
    {
        return std::monostate{};
    }
    return pImpl->Part02();
}

#if AOC_Y2021_CONSTEXPR_UNIT_TEST
namespace
{
consteval bool TestDay01()
{

    std::vector<int> exampleInput = {
        199, 200, 208, 210, 200, 207, 240, 269, 260, 263,
    };
    PuzzleDay01Impl impl{std::move(exampleInput)};

    if (7u != impl.Part01())
    {
        return false;
    }

    return 5u == impl.Part02();
}

static_assert(TestDay01(), "");
} // namespace
#endif // AOC_Y2021_CONSTEXPR_UNIT_TEST

} // namespace AOC::Y2021
