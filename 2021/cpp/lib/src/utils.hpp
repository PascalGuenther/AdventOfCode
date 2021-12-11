#if !defined(AOC_Y2021_UTILS_HPP)
#define AOC_Y2021_UTILS_HPP (1)

#include "../inc/types.hpp"

#include <array>
#include <cstdint>
#include <string_view>
#include <type_traits>
#include <vector>

namespace AOC::Y2021
{
std::vector<int> ParseLineByLineInt(std::string_view str);

template <typename T> constexpr T ParseNumber(std::string_view str, const std::uint8_t base = 10u)
{
    T ret{0};
    [[maybe_unused]] bool bNegative = false;
    bool bParsedFirstDigit = false;
    for (; str.size(); str.remove_prefix(1u))
    {
        const auto c = str.front();
        if (!bParsedFirstDigit)
        {
            switch (c)
            {
            case '-':
                if constexpr (std::is_signed_v<std::decay_t<T>>)
                {
                    bNegative = true;
                    continue;
                }
                else
                {
                    return ret;
                }
            case '+':
            case ' ':
                continue;

            default:
                break;
            }
        }
        T digit = 0;
        if ((c >= '0') && (c <= '9'))
        {
            digit = c - '0';
        }
        else if ((base > 10) && ((c >= 'a') && (c <= 'z')))
        {
            digit = c - 'a' + 10;
        }
        else if ((base > 10) && ((c >= 'A') && (c <= 'Z')))
        {
            digit = c - 'A' + 10;
        }
        else
        {
            break;
        }
        if (digit >= base)
        {
            break;
        }
        bParsedFirstDigit = true;
        ret *= base;
        ret += digit;
    }
    if constexpr (std::is_signed_v<std::decay_t<T>>)
    {
        if (bNegative)
        {
            ret *= -1;
        }
    }
    return ret;
}
static_assert(5 == ParseNumber<int>("5"));
static_assert(50 == ParseNumber<int>(" 50"));
static_assert(450 == ParseNumber<int>(" +   450fdsgs"));
static_assert(-21 == ParseNumber<int>("- 21"));
static_assert(-14 == ParseNumber<int>("- 1110", 2));
static_assert(0xFFFF == ParseNumber<unsigned int>(" +  FFFF", 16));
static_assert(0xDEADBEEFul == ParseNumber<unsigned int>(" +  DeADbEEF", 16));

template <typename T, std::size_t N> consteval auto Vector2Array(const std::vector<T> &vec)
{
    std::array<T, N> ret{};
    std::copy(vec.begin(), vec.end(), ret.begin());
    return ret;
}

} // namespace AOC::Y2021

#endif // !defined(AOC_Y2021_UTILS_HPP)
