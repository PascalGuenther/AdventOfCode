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
#include <variant>
#include <vector>

#include "NumberConversions.hpp"
#include "fileUtils.hpp"

namespace
{

using std::size_t;
using std::uint64_t;

constexpr std::string_view aExampleInput1[] = {
    "mask = XXXXXXXXXXXXXXXXXXXXXXXXXXXXX1XXXX0X",
    "mem[8] = 11",
    "mem[7] = 101",
    "mem[8] = 0",
};

constexpr std::string_view aExampleInput2[] = {
    "mask = 000000000000000000000000000000X1001X",
    "mem[42] = 100",
    "mask = 00000000000000000000000000000000X0XX",
    "mem[26] = 1",
};

class Decoder
{
public:
    bool ProcessProgramLine(const std::string_view &line)
    {
        constexpr char prefixMask[] = "mask = ";
        constexpr char prefixMem[] = "mem[";
        if (0 == line.find(prefixMask) && (line.size() == sizeof(prefixMask) - sizeof(static_cast<char>('\0')) + s_bitmaskLen))
        {
            const auto maskSV = line.substr(sizeof(prefixMask) - sizeof(static_cast<char>('\0')));
            if (s_bitmaskLen != maskSV.size())
            {
                return false;
            }
            m_mask0 = 0u;
            m_mask1 = 0u;
            m_maskX = 0u;
            for (const auto &bit : maskSV)
            {
                m_mask0 <<= 1;
                m_mask1 <<= 1;
                m_maskX <<= 1;
                switch (bit)
                {
                    case '0':
                        m_mask0 |= 1;
                        break;
                    case '1':
                        m_mask1 |= 1;
                        break;
                    case 'X':
                        m_maskX |= 1;
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
            this->StoreValue(addr, value);
            return true;
        }
        return false;
    }

    virtual void StoreValue(uint64_t addr, uint64_t value) = 0;

    [[nodiscard]] uint64_t CalculateMemorySum() const
    {

        return std::accumulate(m_memory.begin(), m_memory.end(), 0ull,
                               [](const auto acc, const auto &mem) { return acc + mem.second; });
    }

protected:
    constexpr static size_t s_bitmaskLen{36u};
    uint64_t m_mask0{};
    uint64_t m_mask1{};
    uint64_t m_maskX{};
    std::map<uint64_t, uint64_t> m_memory;
};
class Decoder1 final : public Decoder
{
public:
    void StoreValue(uint64_t addr, uint64_t value) override
    {
        value |= m_mask1;
        value &= ~m_mask0;
        m_memory[addr] = value;
    }
};

class Decoder2 final : public Decoder
{
public:
    void StoreValue(uint64_t addr, uint64_t value) override
    {
        addr |= m_mask1;
        constexpr uint64_t one64Bit{1u};
        const size_t maxPermutations = one64Bit << AOC::Popcount(m_maskX);
        for (uint64_t permutation = 0u; permutation < maxPermutations; ++permutation)
        {
            uint64_t localMask0{0};
            uint64_t localMask1{0};
            size_t bit{0};
            for (size_t pos = 0; pos < s_bitmaskLen; pos++)
            {
                if (m_maskX & (one64Bit << pos))
                {
                    if (permutation & (one64Bit << bit))
                    {
                        localMask1 |= one64Bit << pos;
                    }
                    else
                    {
                        localMask0 |= one64Bit << pos;
                    }
                    bit++;
                }
            }
            addr |= localMask1;
            addr &= ~localMask0;
            m_memory[addr] = value;
        }

        m_memory[addr] = value;
    }
};

int Day14_Part1(const std::vector<std::string> &input)
{
    std::cout << "=Part 1=\n";
    Decoder1 computer;
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

int Day14_Part2(const std::vector<std::string> &input)
{
    std::cout << "=Part 1=\n";
    Decoder2 computer;
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

} // namespace

int main(const int argc, const char *const argv[])
{
    std::cout << "==Day 14==\n";
#if !defined(NDEBUG)
    std::cout << "Debug build\n";
#endif
    try
    {
        const std::array<std::vector<std::string>, 2> rawInput = [&]() -> std::array<std::vector<std::string>, 2> {
            std::array<std::vector<std::string>, 2> input;
            if (argc > 1)
            {
                auto inputVariant = OpenFile(argv[1u]);
                if (std::holds_alternative<std::ifstream>(inputVariant))
                {
                    auto &file = std::get<std::ifstream>(inputVariant);
                    std::string line;
                    while (std::getline(file, line))
                    {
                        input[0].push_back(line);
                    }
                    input[1] = input[0];
                    return input;
                }
                std::cerr << "Failed to open or parse input file";
            }
            std::cout << "Using example input\n";
            for (const auto &line : aExampleInput1)
            {
                input[0].emplace_back(line);
            }
            for (const auto &line : aExampleInput2)
            {
                input[1].emplace_back(line);
            }
            return input;
        }();

        int ret = Day14_Part1(rawInput[0]);
        if (ret == 0)
        {
            ret = Day14_Part2(rawInput[1]);
        }
        return ret;
    }
    catch (...)
    {
        std::cerr << "Failed to read or parse input\n";
        return 1;
    }
}
