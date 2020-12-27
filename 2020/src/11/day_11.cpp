
#include <algorithm>
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <iterator>
#include <string>
#include <string_view>
#include <variant>
#include <vector>


#include "fileUtils.hpp"

namespace
{

using std::size_t;

constexpr std::string_view exampleInput[]{
    "L.LL.LL.LL", "LLLLLLL.LL", "L.L.L..L..", "LLLL.LL.LL", "L.LL.LL.LL",
    "L.LLLLL.LL", "..L.L.....", "LLLLLLLLLL", "L.LLLLLL.L", "L.LLLLL.LL",
};

#define AOC_POSITION_TYPE_LIST(_X_) _X_(floor, ".") _X_(emptySeat, "L") _X_(occupiedSeat, "#")

enum class GridElement : char
{
#define AOC_POSITION_TYPE_ENUM_ENTRY(name, symbol) name = symbol[0u],
    AOC_POSITION_TYPE_LIST(AOC_POSITION_TYPE_ENUM_ENTRY)
};

struct Grid
{
    size_t width{0u};
    std::vector<GridElement> elems;

    bool RunSimulationStep()
    {
        const auto prevElems = this->elems;
        const size_t height = elems.size() / width;
        assert(height * width == elems.size());
        for (size_t y = 0; y < height; y++)
        {
            for (size_t x = 0; x < width; x++)
            {
                const auto offset = y * width;
                const auto prevState = prevElems[offset + x];
                switch (prevState)
                {
                    case GridElement::emptySeat:
                        [[fallthrough]];
                    case GridElement::occupiedSeat:
                        break;
                    default:
                        continue;
                }
                const auto searchRowBegin = (y > 0) ? (y - 1) : 0;
                const auto searchRowEnd = (y + 2 > height) ? height : (y + 2);
                const auto searchColumnBegin = (x > 0) ? (x - 1) : 0;
                const auto searchColumnEnd = (x + 2 > width) ? width : (x + 2);
                size_t occupiedNeighborsCnt{0u};
                for (auto searchRow = searchRowBegin; searchRow < searchRowEnd; searchRow++)
                {
                    for (auto searchColumn = searchColumnBegin; searchColumn < searchColumnEnd; searchColumn++)
                    {
                        if ((y == searchRow) && (x == searchColumn))
                        {
                            continue;
                        }
                        const auto pos = (width * searchRow) + searchColumn;
                        if (GridElement::occupiedSeat == prevElems[pos])
                        {
                            occupiedNeighborsCnt++;
                        }
                    }
                }
                if (prevState == GridElement::emptySeat)
                {
                    if (occupiedNeighborsCnt == 0)
                    {
                        elems[offset + x] = GridElement::occupiedSeat;
                    }
                }
                else if (prevState == GridElement::occupiedSeat)
                {
                    if (occupiedNeighborsCnt >= 4)
                    {
                        elems[offset + x] = GridElement::emptySeat;
                    }
                }
            }
        }
        return (this->elems != prevElems);
    }

    [[nodiscard]] unsigned int CountSeatsOfState(const GridElement state) const
    {
        return static_cast<unsigned int>(std::count(elems.begin(), elems.end(), state));
    }

    void Print() const
    {
        const auto end = elems.size();
        for (size_t i = 0; i < end; i++)
        {
            if (i % width == 0)
            {
                std::cout << '\n';
            }
            std::cout << static_cast<char>(elems[i]);
        }
        std::cout << "\n\n";
    }
};

int Day11_Part1(const Grid &input)
{
    std::cout << "=Part 1=\n";
    Grid inputCopy{input};
    do
    {
#if !defined(NDEBUG)
        // inputCopy.Print();
        static int iteration{0u};
        std::cout << "Iteation " << ++iteration << "\n";
#endif
    } while (inputCopy.RunSimulationStep());
    const auto occupiedCnt = inputCopy.CountSeatsOfState(GridElement::occupiedSeat);
    std::cout << "In the stable state, " << occupiedCnt << " seats are occupied\n";

    return 0;
}

int Day11_Part2(const Grid & /* input */)
{
    std::cout << "=Part 2=\n";
    std::cout << "To be implemented...\n";
    return 0;
}

[[nodiscard]] Grid ParseRawInput(const std::vector<std::string> &rawInput)
{
    Grid g;
    for (const auto &line : rawInput)
    {
        if (g.width == 0)
        {
            g.width = line.size();
        }
        else if (g.width != line.size())
        {
            throw;
        }
        for (const auto &c : line)
        {
            switch (c)
            {
#define AOC_POSITION_TYPE_PARSER_CASE(name, symbol)                                                                              \
    case symbol[0u]:                                                                                                             \
        g.elems.push_back(GridElement::name);                                                                                    \
        break;
                AOC_POSITION_TYPE_LIST(AOC_POSITION_TYPE_PARSER_CASE)

                default:
                    throw;
            }
        }
    }
    return g;
}

} // namespace

int main(const int argc, const char *const argv[])
{
    std::cout << "==Day 11==\n";
#if !defined(NDEBUG)
    std::cout << "Debug build\n";
#endif
    try
    {
        const auto rawInput = [&]() -> std::vector<std::string> {
            std::vector<std::string> lines;
            if (argc > 1)
            {
                auto inputVariant = OpenFile(argv[1u]);
                if (std::holds_alternative<std::ifstream>(inputVariant))
                {
                    auto &file = std::get<std::ifstream>(inputVariant);
                    std::string line;
                    while (std::getline(file, line))
                    {
                        lines.push_back(line);
                    }
                    return lines;
                }
                std::cerr << "Failed to open or parse input file";
            }
            std::cout << "Using example input\n";
            for (const auto &line : exampleInput)
            {
                lines.emplace_back(line);
            }
            return lines;
        }();

        const auto input = ParseRawInput(rawInput);

        int ret = Day11_Part1(input);
        if (ret == 0)
        {
            ret = Day11_Part2(input);
        }
        return ret;
    }
    catch (...)
    {
        std::cerr << "Failed to read or parse input\n";
        return 1;
    }
}
