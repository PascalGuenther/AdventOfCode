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
#include <variant>
#include <vector>

#include "NumberConversions.hpp"
#include "fileUtils.hpp"

namespace
{

using std::size_t;
using std::uint64_t;

constexpr std::string_view aExampleInput[] = {
    "mask = XXXXXXXXXXXXXXXXXXXXXXXXXXXXX1XXXX0X",
    "mem[8] = 11",
    "mem[7] = 101",
    "mem[8] = 0",
};

class Computer
{
public:
    bool ProcessProgramLine(const std::string_view &line)
    {
        constexpr size_t bitmaskLen = 36;
        constexpr char prefixMask[] = "mask = ";
        constexpr char prefixMem[] = "mem[";
        if (0 == line.find(prefixMask) && (line.size() == sizeof(prefixMask) - sizeof(static_cast<char>('\0')) + bitmaskLen))
        {
            const auto maskSV = line.substr(sizeof(prefixMask) - sizeof(static_cast<char>('\0')));
            if (bitmaskLen != maskSV.size())
            {
                return false;
            }
            m_maskSet = 0;
            m_maskReset = 0;
            for (const auto &bit : maskSV)
            {
                m_maskSet <<= 1;
                m_maskReset <<= 1;
                switch (bit)
                {
                    case '1':
                        m_maskSet |= 1;
                        break;
                    case '0':
                        m_maskReset |= 1;
                        break;
                    case 'X':
                        break;

                    default:
                        return false;
                }
            }
            return true;
        }
        else if (0 == line.find(prefixMem))
        {
            constexpr auto addrBeginPos = sizeof(prefixMem) - sizeof(static_cast<char>('\0'));
            const auto addrEndPos = line.find(']', addrBeginPos);
            if (std::string_view::npos == addrEndPos)
            {
                return false;
            }
            uint64_t addr{0};
            if (!AOC::ReadFromSv(addr, line, addrBeginPos, addrEndPos))
            {
                return false;
            }

            constexpr char sequenceEquals[] = "= ";
            const auto equalsPos = line.find(sequenceEquals);
            if (std::string_view::npos == equalsPos)
            {
                return false;
            }
            const auto valueBeginPos = equalsPos + sizeof(sequenceEquals) - sizeof(static_cast<char>('\0'));
            const auto valueEndPos = line.size();
            if (valueBeginPos >= valueEndPos)
            {
                return false;
            }
            uint64_t value{0};
            if (!AOC::ReadFromSv(value, line, valueBeginPos, valueEndPos))
            {
                return false;
            }
            value |= m_maskSet;
            value &= ~m_maskReset;

            m_memory[addr] = value;
            return true;
        }
        return false;
    }

    [[nodiscard]] uint64_t CalculateMemorySum() const
    {

        return std::accumulate(m_memory.begin(), m_memory.end(), 0ull,
                               [](const auto acc, const auto &mem) { return acc + mem.second; });
    }

private:
    uint64_t m_maskSet{};
    uint64_t m_maskReset{};
    std::map<uint64_t, uint64_t> m_memory;
};

int Day14_Part1(const std::vector<std::string> &input)
{
    std::cout << "=Part 1=\n";
    Computer computer;
    for (const auto &line : input)
    {
        if (!computer.ProcessProgramLine(line))
        {
            return 1;
        }
    }
    const auto memSum = computer.CalculateMemorySum();
    std::cout << "Sum of all values left in memory: " << memSum << "\n";
    return 0;
}

int Day14_Part2(const std::vector<std::string> & /* input */)
{
    std::cout << "=Part 2=\n";

    return 0;
}

} // namespace

int main(const int argc, const char *const argv[])
{
    std::cout << "==Day 14==\n";
#if !defined(NDEBUG)
    std::cout << "Debug build\n";
#endif
    try
    {
        const std::vector<std::string> rawInput = [&]() -> std::vector<std::string> {
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
            for (const auto &line : aExampleInput)
            {
                lines.emplace_back(line);
            }
            return lines;
        }();

        int ret = Day14_Part1(rawInput);
        if (ret == 0)
        {
            ret = Day14_Part2(rawInput);
        }
        return ret;
    }
    catch (...)
    {
        std::cerr << "Failed to read or parse input\n";
        return 1;
    }
}
