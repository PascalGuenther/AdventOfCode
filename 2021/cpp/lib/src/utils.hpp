#if !defined(AOC_Y2021_UTILS_HPP)
#define AOC_Y2021_UTILS_HPP (1)

#include "../inc/types.hpp"

#include <string_view>
#include <vector>

namespace AOC::Y2021
{
std::vector<int> ParseLineByLineInt(std::string_view str);

template <typename T> constexpr T ParseNumber(std::string_view str)
{
    T ret{0};
    bool bNegative = false;
    bool bParsedFirstDigit = false;
    for (; str.size(); str.remove_prefix(1u))
    {
        const auto c = str.front();
        if (!bParsedFirstDigit)
        {
            switch (c)
            {
            case '-':
                bNegative = true;
                continue;
            case '+':
            case ' ':
                continue;

            default:
                break;
            }
        }
        if ((c < '0') || (c > '9'))
        {
            break;
        }
        ret *= 10;
        ret += c - '0';
        bParsedFirstDigit = true;
    }
    if (bNegative)
    {
        ret *= -1;
    }
    return ret;
}
static_assert(5 == ParseNumber<int>("5"));
static_assert(50 == ParseNumber<int>(" 50"));
static_assert(450 == ParseNumber<int>(" +450fdsgs"));
static_assert(-21 == ParseNumber<int>("- 21"));


} // namespace AOC::Y2021

#endif // !defined(AOC_Y2021_UTILS_HPP)
