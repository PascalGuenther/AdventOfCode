#include <algorithm>
#include <cassert>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

class Seat
{
public:
    using seatId_t = std::uint16_t;

public:
    [[nodiscard]] seatId_t GetId() const { return m_seatId; }

    Seat(const std::string_view &boardingPass) : m_seatId(ParseBoardingPass(boardingPass)) {}

private:
    static seatId_t ParseBoardingPass(const std::string_view &boardingPass)
    {
        if (boardingPass.size() != s_seatIdDigits)
        {
            assert(false);
            return 0;
        }
        seatId_t seatIdDigits{};
        for (const char &c : boardingPass)
        {
            seatIdDigits <<= 1u;
            static_assert(8u * sizeof(m_seatId) > s_seatIdDigits, "");
            switch (c)
            {
                case 'B':
                    [[fallthrough]];
                case 'R':
                    seatIdDigits |= 0b1u;
                    break;

                case 'F':
                    [[fallthrough]];
                case 'L':
                    break;

                default:
                    assert(false);
                    return 0;
            }
        }
        return seatIdDigits;
    }

public:
    static constexpr size_t s_seatIdDigits = 10u;

private:
    seatId_t m_seatId{};
};

[[nodiscard]] int ReadBoardingPassFile(const char *filename)
{
    if (nullptr == filename)
    {
        return 1;
    }
    std::ifstream inputFile(filename, std::ifstream::in);
    if (!inputFile.good())
    {
        return 1;
    }
    std::vector<Seat> assignedSeats;
    std::string line;
    while (std::getline(inputFile, line))
    {
        if (!line.empty())
        {
            assignedSeats.emplace_back(line);
        }
    }
    std::sort(assignedSeats.begin(), assignedSeats.end(),
              [](const auto &lhs, const auto &rhs) { return (lhs.GetId() < rhs.GetId()); });
    if (assignedSeats.size() < 1u)
    {
        std::cout << "No seats assigned";
        return 1;
    }
    {
        std::cout << "=Part 1=\n";
        const auto it = std::max_element(assignedSeats.begin(), assignedSeats.end(),
                                         [](const auto &lhs, const auto &rhs) { return (lhs.GetId() < rhs.GetId()); });
        std::cout << "Highest seat ID: " << it->GetId() << "\n";
    }
    {
        std::cout << "=Part 2=\n";
        const auto beforeOurs =
            std::adjacent_find(assignedSeats.begin(), assignedSeats.end(),
                               [](const auto &current, const auto &next) { return ((current.GetId() + 2) == next.GetId()); });
        if (beforeOurs == assignedSeats.end())
        {
            return 1;
        }
        const Seat::seatId_t ourSeatId = beforeOurs->GetId() + 1;
        std::cout << "Our seat ID: " << ourSeatId << "\n";
    }
    return 0;
}

int main(int argc, const char *argv[])
{
    std::cout << "==Day 05==\n";
    if (argc < 2)
    {
        std::cout << "Please specify an input file\n";
        return 1;
    }
    const int ret = ReadBoardingPassFile(argv[1u]);
    if (0 != ret)
    {
        std::cerr << "Failed to read seat IDs from file\n";
        return 1;
    }

    return ret;
}
