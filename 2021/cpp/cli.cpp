#include "aoc_y2021.hpp"

#include <cstdint>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <streambuf>
#include <string>

int main(const int argc, const char *const argv[])
{
    std::cout << "==Advent of Code 2021==\n";
    if (argc < 3)
    {
        const char *const executableName = ((argc > 0) && (argv != nullptr) && (argv[0u] != nullptr)) ? argv[0u] : "";
        std::cout << "usage: " << executableName << " <day> <inputfile>\n";
        std::cout << "example: " << executableName << " 1 input01.txt\n";
        return (argc == 1) ? EXIT_SUCCESS : EXIT_FAILURE;
    }

    const auto day = [&argv]() -> std::uint8_t {
        if (argv == nullptr)
        {
            return 0;
        }
        const auto dayTmp = std::strtol(argv[1u], nullptr, 0);
        if ((dayTmp < 1) || (dayTmp > 24))
        {
            std::cerr << "error: please select a day from 1 and 24\n";
            return 0;
        }
        return dayTmp;
    }();
    if (day == 0)
    {
        return EXIT_FAILURE;
    }

    std::ifstream ifs(argv[2u]);
    if (!ifs.is_open())
    {
        std::cerr << "error: cannot open input file\n";
        return EXIT_FAILURE;
    }
    const std::string input((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());
    ifs.close();
    if (input.empty())
    {
        std::cerr << "error: empty input\n";
        return EXIT_FAILURE;
    }

    auto pPuzzle = AOC::Y2021::PuzzleFactory::CreatePuzzle(day, input);

    if (pPuzzle == nullptr)
    {
        std::cerr << "sorry, no implementation for day " << unsigned(day) << '\n';
        return EXIT_FAILURE;
    }

    const auto resPart1 = pPuzzle->Part1();
    if (!resPart1.has_value())
    {
        std::cerr << "sorry! failed to solve part 1\n!";
        return EXIT_FAILURE;
    }
    else
    {
        std::cout << "result of part 1: " << resPart1.value() << '\n';
    }

    const auto resPart2 = pPuzzle->Part2();
    if (!resPart2.has_value())
    {
        std::cerr << "sorry! failed to solve part 2\n!";
        return EXIT_FAILURE;
    }
    else
    {
        std::cout << "result of part 2: " << resPart2.value() << '\n';
    }

    return EXIT_SUCCESS;
}
