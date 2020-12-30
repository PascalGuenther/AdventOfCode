
#include <cassert>
#include <cmath>
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

constexpr const char *aExampleInput[] = {"F10", "N3", "F7", "R90", "F11"};

enum class Direction
{
    North,
    East,
    South,
    West,
    Max,
};

struct Position2D
{
    int x{0};
    int y{0};

    [[nodiscard]] int ManhattanDistanceTo(const Position2D &other) const
    {
        return (std::abs(this->x - other.x) + std::abs(this->y - other.y));
    }
};

class Ship
{
public:
    enum class Action : char
    {
#define AOC_SHIP_ACTION_LIST(_X_)                                                                                                \
    _X_(moveNorth, 'N')                                                                                                          \
    _X_(moveEast, 'E')                                                                                                           \
    _X_(moveSouth, 'S')                                                                                                          \
    _X_(moveWest, 'W')                                                                                                           \
    _X_(moveForward, 'F')                                                                                                        \
    _X_(turnLeft, 'L')                                                                                                           \
    _X_(turnRight, 'R')

#define AOC_SHIP_ACTION_ENUM_ENTRY(action, symbol) action = symbol,
        AOC_SHIP_ACTION_LIST(AOC_SHIP_ACTION_ENUM_ENTRY)
    };

    enum class ActionType
    {
        move,
        turn,
        Max
    };

public:
    explicit Ship(const Direction orientation, const Position2D position = {0, 0})
        : m_orientation(orientation), m_position(position)
    {
    }

    [[nodiscard]] Position2D GetPositon() const { return m_position; }

    [[nodiscard]] static ActionType GetActionType(const Action action)
    {
        switch (action)
        {
            case Action::moveNorth:
                [[fallthrough]];
            case Action::moveEast:
                [[fallthrough]];
            case Action::moveSouth:
                [[fallthrough]];
            case Action::moveWest:
                [[fallthrough]];
            case Action::moveForward:
                return ActionType::move;
            case Action::turnLeft:
                [[fallthrough]];
            case Action::turnRight:
                return ActionType::turn;
            default:
                assert(false);
                return ActionType::Max;
        }
    }

    bool Navigate(const Action action, const int value)
    {
        const auto actionType = GetActionType(action);

        switch (actionType)
        {
            case ActionType::move:
            {
                int factorX{0};
                int factorY{0};
                Direction direction;
                switch (action)
                {
#define OAC_MOVE_TO_DIRECTION_CASE_BREAK(dir)                                                                                    \
    case Action::move##dir:                                                                                                      \
        direction = Direction::dir;                                                                                              \
        break;
                    OAC_MOVE_TO_DIRECTION_CASE_BREAK(North)
                    OAC_MOVE_TO_DIRECTION_CASE_BREAK(East)
                    OAC_MOVE_TO_DIRECTION_CASE_BREAK(South)
                    OAC_MOVE_TO_DIRECTION_CASE_BREAK(West)
                    case Action::moveForward:
                        direction = m_orientation;
                        break;
                    default:
                        return false;
                }
                switch (direction)
                {
                    case Direction::North:
                        factorX = 0;
                        factorY = -1;
                        break;
                    case Direction::East:
                        factorX = 1;
                        factorY = 0;
                        break;
                    case Direction::South:
                        factorX = 0;
                        factorY = 1;
                        break;
                    case Direction::West:
                        factorX = -1;
                        factorY = 0;
                        break;
                    default:
                        assert(false);
                        return false;
                }
                assert((factorX != 0) || (factorY != 0));
                m_position.x += factorX * value;
                m_position.y += factorY * value;
                return true;
            }

            case ActionType::turn:
            {

                bool turnRight = (action == Action::turnRight) ? true : false;
                if (value < 0)
                {
                    turnRight = !turnRight;
                }
                const auto absValue = std::abs(value);
                if (absValue % 90 != 0)
                {
                    return false;
                }
                static_assert(static_cast<int>(Direction::Max) == 4, "");
                const auto ninetyDegreeTurns = ((absValue / 90) % 4) * (turnRight ? 1 : -1);
                const int newOrientation =
                    (static_cast<int>(m_orientation) + ninetyDegreeTurns + 4) % static_cast<int>(Direction::Max);
                if ((newOrientation < 0) || (newOrientation > static_cast<int>(Direction::Max)))
                {
                    return false;
                }
                m_orientation = static_cast<Direction>(newOrientation);
                return true;
            }

            default:
                assert(false);
                return false;
        }
    }

    bool Navigate(const std::string_view &instruction)
    {
        const size_t numberBeginPos{1u};
        if (instruction.size() < (numberBeginPos + 1u))
        {
            return false;
        }
        Action action = static_cast<Action>(instruction[0u]);
        switch (action)
        {
#define AOC_SHIP_ACTION_CASE(action, symbol)                                                                                     \
    case Action::action:                                                                                                         \
        break;
            AOC_SHIP_ACTION_LIST(AOC_SHIP_ACTION_CASE)
#undef AOC_SHIP_ACTION_CASE

            default:
                return false;
        }
        int value{};
        const auto valueStr = instruction.substr(numberBeginPos);
        bool isFirstChar = true;
        const bool isNegative = (valueStr[0] == '-') ? true : false;
        for (const auto &c : valueStr)
        {
            if (isFirstChar)
            {
                isFirstChar = false;
                switch (c)
                {
                    case '-':
                    case '+':
                        continue;

                    default:
                        break;
                }
            }
            if ((c < '0') || (c > '9'))
            {
                return false;
            }
            value *= 10;
            value += c - '0';
        }
        if (isNegative)
        {
            value *= -1;
        }
        return Navigate(action, value);
    }

private:
    Direction m_orientation;
    Position2D m_position{0, 0};
};

int Day12_Part1(const std::vector<std::string> &input)
{
    std::cout << "=Part 1=\n";
    constexpr Position2D startPos{0, 0};
    Ship ship(Direction::East, startPos);
    for (const auto &instruction : input)
    {
        if (!ship.Navigate(instruction))
        {
            std::cerr << "Failed to parse navigation instruction";
#if !defined(NDEBUG)
            std::cerr << ": " << instruction;
#endif
            std::cerr << "\n";
            return 1;
        }
    }
    std::cout << "Manhattan distance to the start postion: " << ship.GetPositon().ManhattanDistanceTo(startPos) << "\n";
    return 0;
}

} // namespace

int main(const int argc, const char *const argv[])
{
    std::cout << "==Day 12==\n";
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
                        if (!line.empty())
                        {
                            lines.push_back(line);
                        }
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

        int ret = Day12_Part1(rawInput);
        if (ret == 0)
        {
            // ret = Day12_Part2(input);
        }
        return ret;
    }
    catch (...)
    {
        std::cerr << "Failed to read or parse input\n";
        return 1;
    }
}
