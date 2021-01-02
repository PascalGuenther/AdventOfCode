
#pragma once

#include <cstdint>
#include <string_view>
#include <type_traits>

namespace AOC
{

using std::size_t;

template <typename T>
inline bool ReadFromSv(T &out, const std::string_view &in, const size_t beginPos, const size_t endPos, const int base = 10)
{
    if ((base < 0) || (base > 10))
    {
        return false;
    }
    out = 0;
    for (size_t i = beginPos; i < endPos; i++)
    {
		
        if ((in[i] < '0') || (in[i] > ('0' + base)))
        {
            return false;
        }
        out *= base;
        out += in[i] - '0';
    }
    return true;
}

template <typename T>
static size_t Popcount(const T num)
{
    size_t ret{0};
    for (size_t i = 0u; i < 8 * sizeof(num); ++i)
    {
		constexpr T one{1};
        if (num & (one << i))
        {
            ret++;
        }
    }
    return ret;
}

} // namespace AOC
