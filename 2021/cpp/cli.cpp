#include "aoc_y2021.hpp"

#include <cstdint>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <streambuf>
#include <string>

// #define AOC_Y2021_DEBUG_DAY (9)

#if !defined(AOC_Y2021_DEBUG_DAY)
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
        return static_cast<std::uint8_t>(dayTmp);
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
#elif AOC_Y2021_DEBUG_DAY == 3
int main()
{
    constexpr auto day = AOC_Y2021_DEBUG_DAY;
    constexpr auto exampleInput =
        "00100\n11110\r\n10110\r\n10111\r\n10101\r\n01111\r\n00111\r\n11100\n10000\r\n11001\r\n00010\r\n01010\r";
    auto pPuzzle = AOC::Y2021::PuzzleFactory::CreatePuzzle(day, exampleInput);
#elif AOC_Y2021_DEBUG_DAY == 4
int main()
{
    constexpr auto day = AOC_Y2021_DEBUG_DAY;
    constexpr auto exampleInput =
        "7,4,9,5,11,17,23,2,0,14,21,24,10,16,13,6,15,25,12,22,18,20,8,19,3,26,1\n\r\n22 13 17 11  0\r\n 8  2 23  4 "
        "24\r\n21  9 14 16  7\r\n 6 10  3 18  5\r\n 1 12 20 15 19\n\r\n 3 15  0  2 22\r\n 9 18 13 17  5\n19  8  7 25 "
        "23\r\n20 11 10 24  4\n14 21 16 12  6\r\n\n14 21 17 24  4\r\n10 16 15  9 19\r\n18  8 23 26 20\r\n22 11 13  6  "
        "5\r\n 2  0 12  3  7\r";
    auto pPuzzle = AOC::Y2021::PuzzleFactory::CreatePuzzle(day, exampleInput);
#elif AOC_Y2021_DEBUG_DAY == 5
int main()
{
    constexpr auto day = AOC_Y2021_DEBUG_DAY;
    constexpr auto exampleInput = "0,9 -> 5,9\r\n8,0 -> 0,8\n9,4 -> 3,4\n2,2 -> 2,1\r\n7,0 -> 7,4\r\n6,4 -> 2,0\n0,9 "
                                  "-> 2,9\r\n3,4 -> 1,4\r\n0,0 -> 8,8\r\n5,5 -> 8,2";
    auto pPuzzle = AOC::Y2021::PuzzleFactory::CreatePuzzle(day, exampleInput);
#elif AOC_Y2021_DEBUG_DAY == 6
int main()
{
    constexpr auto day = AOC_Y2021_DEBUG_DAY;
    constexpr auto exampleInput = "3,4,3,1,2";
    auto pPuzzle = AOC::Y2021::PuzzleFactory::CreatePuzzle(day, exampleInput);
#elif AOC_Y2021_DEBUG_DAY == 7
int main()
{
    constexpr auto day = AOC_Y2021_DEBUG_DAY;
    constexpr auto exampleInput = "16,1,2,0,4,2,7,1,2,14";
    auto pPuzzle = AOC::Y2021::PuzzleFactory::CreatePuzzle(day, exampleInput);
#elif AOC_Y2021_DEBUG_DAY == 8
int main()
{
    constexpr auto day = AOC_Y2021_DEBUG_DAY;
    constexpr auto exampleInput = "2199943210\n3987894921\n9856789892\n8767896789\n9899965678" auto pPuzzle =
        AOC::Y2021::PuzzleFactory::CreatePuzzle(day, exampleInput);
#elif AOC_Y2021_DEBUG_DAY == 9
int main()
{
    constexpr auto day = AOC_Y2021_DEBUG_DAY;
    constexpr auto exampleInput = "2199943210\n3987894921\n9856789892\n8767896789\n9899965678";
    auto pPuzzle = AOC::Y2021::PuzzleFactory::CreatePuzzle(day, exampleInput);
#else
#error "please specify debug target"
#endif
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
