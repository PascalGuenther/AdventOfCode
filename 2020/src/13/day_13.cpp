
#include <algorithm>
#include <array>
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <iterator>
#include <limits>
#include <string>
#include <string_view>
#include <variant>
#include <vector>

#include "fileUtils.hpp"

namespace
{

using std::size_t;

constexpr const char *aExampleInput[] = {"939", "7,13,x,x,59,x,31,19"};

struct BusSchedule
{
public:
    struct Departure
    {
        int busId;
        int waitTime;
    };

public:
    explicit BusSchedule(const std::string &busIds) : m_busIds(ParseBusIds(busIds)) {}

    [[nodiscard]] Departure FindNextDeparture(const std::string &fromTime)
    {
        const int time = std::stoi(fromTime);
        int earliestDeparture{std::numeric_limits<int>::max()};
        int earliestBus{};
        for (const auto bus : m_busIds)
        {
            if (bus <= 0)
            {
                continue;
            }
            const auto loopIteration = (time / bus) + 1;
            const auto departure = loopIteration * bus;
            if (departure < earliestDeparture)
            {
                earliestDeparture = departure;
                earliestBus = bus;
            }
        }
        if ((earliestDeparture == std::numeric_limits<int>::max()) || (earliestDeparture < time))
        {
            assert(false);
            return {0, 0};
        }
        return {earliestBus, earliestDeparture - time};
    }

    [[nodiscard]] std::uint64_t FindEarliestSubsequentDepartures()
    {
        const auto numOfBuses = m_busIds.size();
        std::uint64_t offset = m_busIds[0u];
        size_t index{1u};
        for (std::uint64_t timestamp = offset;; timestamp += offset)
        {
            do
            {
                if (m_busIds[index] <= 0)
                {
                    index++;
                    continue;
                }

                if (0u == ((timestamp + index) % m_busIds[index]))
                {
                    offset *= m_busIds[index];
                    index++;
                }
                else
                {

                    break;
                }
            } while (index < numOfBuses);
            if (index == numOfBuses)
            {
                return timestamp;
            }
        }
    }

private:
    [[nodiscard]] static std::vector<int> ParseBusIds(const std::string &busIds)
    {
        std::vector<int> parsedBusIds;
        size_t pos1{0};
        size_t pos2{0};
        do
        {
            pos2 = busIds.find(',', pos1);
            if (busIds[pos1] != 'x')
            {
                parsedBusIds.push_back(std::stoi(busIds.substr(pos1, pos2)));
            }
            else
            {
                parsedBusIds.push_back(0);
            }
            pos1 = pos2 + 1;
        } while ((pos2 != std::string::npos && (pos1 < busIds.size())));

        return parsedBusIds;
    }

private:
    const std::vector<int> m_busIds;
};

int Day13_Part1(const std::array<std::string, 2u> &input)
{
    std::cout << "=Part 1=\n";
    BusSchedule schedule{input[1u]};
    const auto nextDeparture = schedule.FindNextDeparture(input[0u]);
    if (nextDeparture.busId == 0)
    {
        std::cerr << "Failed to find earliest departure\n";
        return 1;
    }
    std::cout << "Bus " << nextDeparture.busId << " departs in " << nextDeparture.waitTime << " minute"
              << ((nextDeparture.waitTime == 1) ? ".\n" : "s.\n");
    std::cout << "Product: " << nextDeparture.waitTime * nextDeparture.busId << "\n";
    return 0;
}

int Day13_Part2(const std::array<std::string, 2u> &input)
{
    std::cout << "=Part 2=\n";
    BusSchedule schedule{input[1u]};
    const auto earliestSubsequentDepartures = schedule.FindEarliestSubsequentDepartures();
    if (0 == earliestSubsequentDepartures)
    {
        std::cerr << "Could not find a timestamp such that all buses depart at subsequent minutes.\n";
    }
    std::cout << "Earliest timestaps such that all buses depart at an offset matching their positions: "
              << earliestSubsequentDepartures << "\n";
    static_cast<void>(input);
    return 0;
}

} // namespace

int main(const int argc, const char *const argv[])
{
    std::cout << "==Day 13==\n";
#if !defined(NDEBUG)
    std::cout << "Debug build\n";
#endif
    try
    {
        const std::array<std::string, 2u> rawInput = [&]() -> std::array<std::string, 2u> {
            std::array<std::string, 2u> lines;
            if (argc > 1)
            {
                auto inputVariant = OpenFile(argv[1u]);
                if (std::holds_alternative<std::ifstream>(inputVariant))
                {
                    auto &file = std::get<std::ifstream>(inputVariant);
                    std::string line;
                    size_t lineCnt{0u};
                    while (std::getline(file, line) && (lineCnt < lines.size()))
                    {
                        lines[lineCnt++] = line;
                    }
                    return lines;
                }
                std::cerr << "Failed to open or parse input file";
            }
            std::cout << "Using example input\n";
            lines[0u] = aExampleInput[0u];
            lines[1u] = aExampleInput[1u];
            return lines;
        }();

        int ret = Day13_Part1(rawInput);
        if (ret == 0)
        {
            ret = Day13_Part2(rawInput);
        }
        return ret;
    }
    catch (...)
    {
        std::cerr << "Failed to read or parse input\n";
        return 1;
    }
}
