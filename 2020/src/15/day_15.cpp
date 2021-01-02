#include <array>
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <limits>
#include <map>
#include <numeric>
#include <string>
#include <string_view>
#include <utility>
#include <variant>
#include <vector>


#include "NumberConversions.hpp"
#include "fileUtils.hpp"

namespace
{

using std::size_t;
using std::uint64_t;

constexpr std::string_view aExampleInput[] = {
    "0,3,6", "1,3,2", "2,1,3", "1,2,3", "2,3,1", "3,2,1", "3,1,2",
};

class MemoryGame
{
public:
    MemoryGame(const std::string_view &startingNumbers)
    {
        unsigned int turn{1};
        for (size_t startPos = 0; startPos < startingNumbers.size();)
        {
            auto endPos = startingNumbers.find(',', startPos);
            if (endPos > startingNumbers.size())
            {
                endPos = startingNumbers.size();
            }
            unsigned int number{0};
            if (!AOC::ReadFromSv(number, startingNumbers, startPos, endPos))
            {
                break;
            }
            m_numsAndTurns[number] = turn++;
            startPos = endPos + 1;
        }
        m_startSequenceLen = m_numsAndTurns.size();
    }

    std::pair<unsigned int, unsigned int> SpeakNumber()
    {
        const auto itMaxTurn = std::max_element(m_numsAndTurns.begin(), m_numsAndTurns.end(),
                                                [](const auto &p1, const auto &p2) { return (p1.second < p2.second); });
        const auto maxTurn = itMaxTurn->second;
        const auto turn = maxTurn + 1;

        unsigned int numberSpokenThisTurn;
        if (m_FirstOccurance)
        {
            numberSpokenThisTurn = 0u;
        }
        else
        {
            numberSpokenThisTurn = (turn - 1) - m_prevTurnOfNumber;
        }

        const auto numIt = m_numsAndTurns.find(numberSpokenThisTurn);
        const bool newNumberHasBeenSpokenBefore = (numIt != m_numsAndTurns.end());
        if (newNumberHasBeenSpokenBefore)
        {
            m_prevTurnOfNumber = numIt->second;
            m_FirstOccurance = false;
        }
        else
        {
            m_FirstOccurance = true;
        }
        m_numsAndTurns[numberSpokenThisTurn] = turn;

        return {turn, numberSpokenThisTurn};
    }

private:
    std::map<unsigned int, unsigned int> m_numsAndTurns;
    size_t m_startSequenceLen{};
    unsigned int m_prevTurnOfNumber{};
    bool m_FirstOccurance = true;
};

int Day15_Part1(const std::string_view &input)
{
    std::cout << "=Part 1=\n";
    MemoryGame game{input};
    while (true)
    {
        const auto [turn, spokenNumber] = game.SpeakNumber();
        constexpr unsigned int turnOfInterest{2020};
        if (turn == turnOfInterest)
        {
            std::cout << "Number spoken in turn #" << turn << ": " << spokenNumber << "\n";
            return 0;
        }
    }

    return 0;
}

int Day15_Part2(const std::string_view & /*input*/)
{
    std::cout << "=Part 2=\n";
    std::cout << "Sum of all values left in memory: To be implemented...\n";
    return 0;
}

} // namespace

int main(const int argc, const char *const argv[])
{
    std::cout << "==Day 15==\n";
#if !defined(NDEBUG)
    std::cout << "Debug build\n";
#endif
    try
    {
        std::vector<std::string> input;
        if (argc >= 2)
        {

            auto inputVariant = OpenFile(argv[1u]);
            if (std::holds_alternative<std::ifstream>(inputVariant))
            {
                auto &file = std::get<std::ifstream>(inputVariant);
                std::string line;
                while (std::getline(file, line))
                {
                    input.push_back(line);
                }
            }
        }
        if (input.size() == 0)
        {
            for (const auto &line : aExampleInput)
            {
                input.emplace_back(line);
            }
        }

        int ret{0};
        for (const auto &inputLine : input)
        {

            ret = Day15_Part1(inputLine);
            if (ret == 0)
            {
                ret = Day15_Part2(inputLine);
            }
            if (ret != 0)
            {
                return ret;
            }
        }
        return ret;
    }
    catch (...)
    {
        std::cerr << "Failed to read or parse input\n";
        return 1;
    }
}
