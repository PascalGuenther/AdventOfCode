#include <algorithm>
#include <array>
#include <cstddef>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <iterator>
#include <set>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <variant>
#include <vector>

#include "fileUtils.hpp"

namespace
{

using std::size_t;
using Joltage = std::uint32_t;

constexpr Joltage outletJoltage{0u};
constexpr Joltage maxJoltageStep{3u};

int Day010_Part1(const std::set<Joltage> &input)
{
    std::cout << "=Part 1=\n";
    std::array<Joltage, maxJoltageStep + 1u> joltageSteps{};
    Joltage previous{outletJoltage};
    for (const auto &adapter : input)
    {
        const auto diff = adapter - previous;
        previous = adapter;
        if (diff >= joltageSteps.size())
        {
            std::cerr << "No adapter chain found\n";
            return 1;
        }
        joltageSteps[diff]++;
    }

    joltageSteps[3u]++; // take internal joltage adapter into account

    std::cout << "1-jolt steps: " << joltageSteps[1u] << ", 3-jolt steps: " << joltageSteps[3u] << "\n";
    std::cout << "Product of 1-jolt and 3-jolt steps: " << joltageSteps[1u] * joltageSteps[3u] << "\n";
    return 0;
}

} // namespace

int main(const int argc, const char *const argv[])
{
    std::cout << "==Day 10==\n";
#if !defined(NDEBUG)
    std::cout << "Debug build\n";
#endif
    auto input = [&]() -> std::vector<Joltage> {
        if (argc > 1)
        {
            auto inputVariant = ParseInputFile<Joltage>(argv[1u]);
            if (std::holds_alternative<std::vector<Joltage>>(inputVariant))
            {
                return std::get<std::vector<Joltage>>(inputVariant);
            }
            std::cerr << "Failed to open or parse input file";
        }
        std::cout << "Using example input\n";
        return {
            28, 33, 18, 42, 31, 14, 46, 20, 48, 47, 24, 23, 49, 45, 19, 38, 39, 11, 1, 32, 25, 35, 8, 17, 7, 9, 4, 2, 34, 10, 3,
        };
    }();
    std::set<Joltage> sortedInput;
    for (const auto &i : input)
    {
        sortedInput.insert(i);
    }

    int ret = Day010_Part1(sortedInput);
    return ret;
}
