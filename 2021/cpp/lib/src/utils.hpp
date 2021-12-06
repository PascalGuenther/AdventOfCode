#if !defined(AOC_Y2021_UTILS_HPP)
#define AOC_Y2021_UTILS_HPP (1)

#include "../inc/types.hpp"

#include <string_view>
#include <vector>

namespace AOC::Y2021
{
std::vector<int> ParseLineByLineInt(const std::string_view &str);
}

#endif // !defined(AOC_Y2021_UTILS_HPP)
