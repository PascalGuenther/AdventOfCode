#pragma once

#include <fstream>
#include <string>
#include <type_traits>
#include <variant>
#include <vector>

namespace AOC
{
template <class T>
struct dependend_false : std::false_type
{
};
} // namespace AOC

inline std::variant<std::ifstream, int> OpenFile(const char *const filename)
{
    if (nullptr == filename)
    {
        return 1;
    }

    auto inFile = std::ifstream(filename);
    if (inFile.good())
    {
        return inFile;
    }
    return 1;
}

template <typename T>
inline std::variant<std::vector<T>, int> ParseInputFile(const char *const filename)
{
    auto fileVariant = OpenFile(filename);
    if (std::holds_alternative<int>(fileVariant))
    {
        return std::get<int>(fileVariant);
    }
    try
    {
        std::vector<T> v;
        auto &file = std::get<std::ifstream>(fileVariant);
        for (std::string line; std::getline(file, line);)
        {
            if (line.empty())
            {
                continue;
            }
            if constexpr (std::is_same_v<T, int>)
            {
                v.push_back(std::stoi(line));
            }
            else if constexpr (std::is_same_v<T, long>)
            {
                v.push_back(std::stol(line));
            }
            else if constexpr (std::is_same_v<T, long long>)
            {
                v.push_back(std::stoll(line));
            }
            else if constexpr (std::is_same_v<T, unsigned int>)
            {
                v.push_back(std::stoul(line));
            }
            else if constexpr (std::is_same_v<T, unsigned long>)
            {
                v.push_back(std::stoul(line));
            }
            else if constexpr (std::is_same_v<T, unsigned long long>)
            {
                v.push_back(std::stoull(line));
            }
            else
            {
                static_assert(AOC::dependend_false<T>::value, "Type not supported");
            }
        }
        if (v.size() < 1)
        {
            return 1;
        }
        return v;
    }

    catch (...)
    {
        return 1;
    }
}
