#if !defined(AOC_Y2021_UTILS_HPP)
#define AOC_Y2021_UTILS_HPP (1)

#include "../include/types.hpp"

#include <array>
#include <cstddef>
#include <cstdint>
#include <string_view>
#include <type_traits>
#include <vector>

namespace AOC::Y2021
{
std::vector<int> ParseLineByLineInt(std::string_view str);

constexpr bool ParseLines(std::string_view input, auto &&fnLineCb, const bool exitOnEmptyLine = true)
{
    while (!input.empty())
    {
        const auto rfOrLn = input.find_first_of("\r\n");
        const auto line = (rfOrLn == input.npos) ? input.substr(0) : input.substr(0, rfOrLn);
        if (exitOnEmptyLine && line.empty())
        {
            return false;
        }
        if (!fnLineCb(line))
        {
            return false;
        }
        const auto lf = input.find('\n');
        if (lf == input.npos)
        {
            return true;
        }
        input.remove_prefix(lf + 1u);
    }
    return true;
}

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

template <typename T> AOC_Y2021_CONSTEXPR std::vector<T> ParseCSV2Vector(std::string_view csv)
{
    std::vector<T> ret{};
    for (std::size_t start = 0; csv.size(); csv.remove_prefix(start + 1))
    {
        ret.push_back(ParseNumber<T>(csv));
        start = csv.find_first_of(',');
        if (start == csv.npos)
        {
            break;
        }
    }
    return ret;
}

template <typename T, std::size_t N> consteval auto Vector2Array(const std::vector<T> &vec)
{
    std::array<T, N> ret{};
    std::copy(vec.begin(), vec.end(), ret.begin());
    return ret;
}

template <typename T> class Vector2D
{
  public:
    AOC_Y2021_CONSTEXPR Vector2D(std::size_t width, std::vector<T> &&vec) : m_width(width), m_vec(vec)
    {
    }

    AOC_Y2021_CONSTEXPR inline T &operator()(std::size_t col, std::size_t row)
    {
        return m_vec[m_width * row + col];
    }

    AOC_Y2021_CONSTEXPR inline T operator()(std::size_t col, std::size_t row) const
    {
        return m_vec[m_width * row + col];
    }

    AOC_Y2021_CONSTEXPR inline auto width() const
    {
        return m_width;
    }

    AOC_Y2021_CONSTEXPR inline auto height() const
    {
        const auto w = width();
        const auto s = size();
        const std::size_t h = (w != 0) ? (s / w) : ((s == 0) ? 0 : 1);
        return h;
    }

    AOC_Y2021_CONSTEXPR inline auto size() const
    {
        return m_vec.size();
    }

    AOC_Y2021_CONSTEXPR inline auto empty() const
    {
        return m_vec.empty();
    }

    AOC_Y2021_CONSTEXPR inline auto begin() const
    {
        return m_vec.begin();
    }

    AOC_Y2021_CONSTEXPR inline auto end() const
    {
        return m_vec.end();
    }

  private:
    std::size_t m_width;
    std::vector<T> m_vec;
};

} // namespace AOC::Y2021

#endif // !defined(AOC_Y2021_UTILS_HPP)
