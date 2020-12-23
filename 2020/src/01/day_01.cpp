#include "input_2020_01_a.h"

#include <iostream>
#include <algorithm>
#include <array>
#include <optional>
#include <utility>

using num_t = std::uint32_t;
using std::size_t;

template <size_t Size>
std::optional<std::pair<num_t, num_t>> FindExpenseReportEntries(const num_t (&aUnsortedInput)[Size], num_t expectedSum)
{
    const std::array<num_t, Size> aSortedInput = [&aUnsortedInput]() {
        std::array<num_t, Size> aToSort{};
        for (size_t i = 0u; i < Size; ++i)
        {
            aToSort[i] = aUnsortedInput[i];
        }
        std::sort(aToSort.begin(), aToSort.end());
        return aToSort;
    }();

    auto it = std::find_if(aSortedInput.begin(), aSortedInput.end(), [&](const num_t lhs) {
        auto it2 = std::find_if(aSortedInput.begin(), aSortedInput.end(),
                                [lhs, expectedSum](const num_t rhs) { return (expectedSum == (lhs + rhs)); });
        return it2 != aSortedInput.end();
    });
    if (it == aSortedInput.end())
    {
        return std::nullopt;
    }
    return std::make_pair(*it, expectedSum - (*it));
}

int main()
{
    constexpr num_t expectedSum = 2020u;
    constexpr num_t aUnsortedInput[] = {AOC_INPUT_2020_01_A};
    auto entryPair = FindExpenseReportEntries(aUnsortedInput, expectedSum);
    if (!entryPair.has_value())
    {
        std::cout << "No entry pair found\n";
        return 1;
    }
    std::cout << "Found entries: [" << entryPair.value().first << ", " << entryPair.value().second << "]\n";
    std::cout << "Product of these entries: " << entryPair.value().first * entryPair.value().second << "\n";
    return 0;
}
