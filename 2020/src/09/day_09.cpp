#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <iterator>
#include <optional>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <vector>

namespace
{

using std::size_t;

struct XMAS
{
    using num_t = std::uint64_t;
    const size_t preambleLength;
    const std::vector<num_t> sequence;
};

[[nodiscard]] std::optional<std::pair<size_t, size_t>> FindPositionOfSummands(XMAS::num_t sum, const size_t startIndex,
                                                                              const XMAS &input)
{
    try
    {
        const auto itBegin = input.sequence.begin() + startIndex;
        const auto itEnd = itBegin + input.preambleLength;
        for (auto itFirst = itBegin; itFirst != (itEnd - 1); ++itFirst)
        {
            const auto firstVal = *itFirst;
            if (firstVal > sum)
            {
                continue;
            }
            const auto itSecond = std::find(itFirst + 1, itEnd, sum - firstVal);
            if (itSecond != itEnd)
            {
                const auto firstIndex = static_cast<size_t>(std::distance(input.sequence.begin(), itFirst));
                const auto secondIndex = static_cast<size_t>(std::distance(input.sequence.begin(), itSecond));
                return {{firstIndex, secondIndex}};
            }
        }
        return std::nullopt;
    }
    catch (std::out_of_range & /*e*/)
    {
        return std::nullopt;
    }
}

[[nodiscard]] std::optional<size_t> FindIndexOfFirstInvalidNumber(const XMAS &input)
{
    if (input.preambleLength >= input.sequence.size())
    {
        return std::nullopt;
    }
    try
    {
        const auto lastIndex = input.sequence.size();
        for (size_t i = input.preambleLength; i < lastIndex; ++i)
        {
            if (!FindPositionOfSummands(input.sequence[i], i - input.preambleLength, input).has_value())
            {
                return i;
            }
        }
        return std::nullopt;
    }
    catch (...)
    {
        return std::nullopt;
    }
}

int Day09_Part1(const XMAS &input)
{
    std::cout << "=Part 1=\n";
    const auto invalidNumIndex = FindIndexOfFirstInvalidNumber(input);
    const auto idx = invalidNumIndex.value_or(0);
    if (!invalidNumIndex.has_value() || (idx >= input.sequence.size()))
    {
        std::cerr << "Failed to find an invalid number\n";
        return 2;
    }
    std::cout << "Found first invalid number at index [" << idx << "]: " << input.sequence[idx] << "\n";
    return 0;
}

[[nodiscard]] XMAS LoadDataFromFile(const char *const filename)
{
    if (nullptr == filename)
    {
        throw std::runtime_error("Filename is NULL");
    }
    std::ifstream inputFile(filename, std::ifstream::in);
    if (!inputFile.good())
    {
        throw std::runtime_error("Failed to open input file");
    }
    std::vector<XMAS::num_t> sequence;
    std::string line;
    while (std::getline(inputFile, line))
    {
        if (line.empty())
        {
            continue;
        }
        const auto parsedValue = std::stoull(line);
        sequence.push_back(parsedValue);
        static_assert(std::is_same_v<std::decay_t<decltype(*sequence.begin())>, std::decay_t<decltype(parsedValue)>>, "");
    }
    return {25, sequence};
}

} // namespace

int main(const int argc, const char *const argv[])
{
    std::cout << "==Day 09==\n";
#if !defined(NDEBUG)
    std::cout << "Debug build\n";
#endif
    int ret = 0;
    try
    {
        const XMAS input = [&]() -> XMAS {
            if (argc < 2)
            {
                std::cout
                    << "Using example input. Alternatively, you can pass the path to an input file as command line argument\n";
                return {
                    5,
                    {
                        35, 20, 15, 25, 47, 40, 62, 55, 65, 95, 102, 117, 150, 182, 127, 219, 299, 277, 309, 576,
                    },
                };
            }
            else
            {
                return LoadDataFromFile(argv[1u]);
            }
        }();
        ret = Day09_Part1(input);
    }
    catch (std::runtime_error &e)
    {
        std::cerr << e.what() << '\n';
        return 1;
    }
    catch (...)
    {
        std::cerr << "Failed to load data\n";
        return 1;
    }

    return ret;
}
