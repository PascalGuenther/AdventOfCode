#include "utils.hpp"

#include <cstdlib>
#include <type_traits>
#include <vector>

namespace AOC::Y2021
{

namespace
{

template <typename T> std::vector<T> ParseLineByLineX(const std::string_view &str)
{
    std::vector<T> vec;
    std::size_t startPos = 0u;
    const auto strSize = str.size();
    while (startPos < strSize)
    {
        if constexpr (std::is_signed_v<T>)
        {
            vec.push_back(static_cast<T>(strtoll(str.data() + startPos, nullptr, 0)));
        }
        else
        {
            vec.push_back(static_cast<T>(strtoull(str.data() + startPos, nullptr, 0)));
        }
        startPos = str.find_first_of('\n', startPos);
        if (startPos == str.npos)
        {
            break;
        }
        startPos++;
    }
    return vec;
}

} // namespace

std::vector<int> ParseLineByLineInt(const std::string_view &str)
{
    return ParseLineByLineX<int>(str);
}

} // namespace AOC::Y2021
