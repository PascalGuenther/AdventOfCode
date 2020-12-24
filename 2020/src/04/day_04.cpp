#include <algorithm>
#include <array>
#include <cassert>
#include <fstream>
#include <iostream>
#include <optional>
#include <string>
#include <utility>
#include <vector>

class Passport
{
public:
#define AOC_PASSPORT_FIELD_LIST(_X_)                                                                                             \
    _X_(byr)                                                                                                                     \
    _X_(iyr)                                                                                                                     \
    _X_(eyr)                                                                                                                     \
    _X_(hgt)                                                                                                                     \
    _X_(hcl)                                                                                                                     \
    _X_(ecl)                                                                                                                     \
    _X_(pid)                                                                                                                     \
    _X_(cid)

    enum Field
    {
#define AOC_PASSPORT_FIELD_ENUM_ENTRY(entry) entry,
        AOC_PASSPORT_FIELD_LIST(AOC_PASSPORT_FIELD_ENUM_ENTRY) Max,
    };
    static constexpr const char *aFieldName[] = {
#define AOC_PASSPORT_FIELD_NAME(entry) #entry,
        AOC_PASSPORT_FIELD_LIST(AOC_PASSPORT_FIELD_NAME)};
    static_assert(sizeof(aFieldName) / sizeof(*aFieldName) == Field::Max, "");

public:
    Passport(const std::string_view &keyValuePairs)
    {
        std::array<std::string_view, Field::Max> aKeyValueTokens;
        size_t tokenCnt = 0u;

        std::vector<std::string_view> output;
        size_t tokenBeginPos = 0;

        while (tokenBeginPos < keyValuePairs.size())
        {
            const auto tokenEndPos = keyValuePairs.find_first_of(" ", tokenBeginPos);

            if (tokenBeginPos != tokenEndPos)
            {
                aKeyValueTokens[tokenCnt++] = keyValuePairs.substr(tokenBeginPos, tokenEndPos - tokenBeginPos);
            }

            if ((tokenCnt >= aKeyValueTokens.max_size()) || (tokenEndPos == std::string_view::npos))
            {
                break;
            }
            tokenBeginPos = tokenEndPos + 1u;
        }

        for (size_t i = 0u; i < tokenCnt; ++i)
        {
            auto it = std::find_if(std::begin(aFieldName), std::end(aFieldName), [i, &aKeyValueTokens](const char *name) {
                constexpr char delimiter = ':';
                const auto position = aKeyValueTokens[i].find_first_of(delimiter);
                std::string_view key = aKeyValueTokens[i].substr(0, position);
                return (0 == key.compare(name));
            });
            if (it != std::end(aFieldName))
            {
                size_t distance = std::distance(aFieldName, it);
                if (distance < (sizeof(aFieldName) / sizeof(*aFieldName)))
                {
                    m_fieldsBitmask |= 1u << distance;
                }
                else
                {
                    assert(false);
                }
            }
            else
            {
                assert(false);
            }
        }
    }

    [[nodiscard]] bool IsValid() const
    {
        for (size_t i = 0u; i < Field::Max; i++)
        {
            if (i == Field::cid)
            {
                // ignore this field
                continue;
            }
            if (!(m_fieldsBitmask & (1u << i)))
            {
                // mandatory field is missing
                return false;
            }
        }
        return true;
    }

private:
    std::uint8_t m_fieldsBitmask{};
    static_assert((8 * sizeof(m_fieldsBitmask)) <= Field::Max, "insufficient storage for bit mask");
};

[[nodiscard]] std::optional<std::vector<Passport>> ReadPassportFile(const char *filename)
{
    if (nullptr == filename)
    {
        return std::nullopt;
    }
    std::ifstream inputFile(filename, std::ifstream::in);
    if (!inputFile.good())
    {
        return std::nullopt;
    }
    std::vector<Passport> parsedPassports;
    std::string record;
    std::string line;
    while (std::getline(inputFile, line))
    {
        if (line.empty())
        {
            if (!record.empty())
            {
                parsedPassports.emplace_back(record);
                record = "";
            }
        }
        else
        {
            record += " " + line;
        }
    }
    if (!record.empty())
    {
        parsedPassports.emplace_back(record);
    }
    if (parsedPassports.empty())
    {
        return std::nullopt;
    }
    return parsedPassports;
}

int main(int argc, const char *argv[])
{
    std::cout << "==Day 04==\n";
    if (argc < 2)
    {
        std::cout << "Please specify an input file\n";
        return 1;
    }
    const auto parsedPassports = ReadPassportFile(argv[1u]);
    if (!parsedPassports.has_value())
    {
        std::cerr << "Failed to read passports from file\n";
        return 1;
    }

    const auto &passports = parsedPassports.value();
    {
        std::cout << "=Part 1=\n";
        const auto numberOfValidPassports =
            std::count_if(passports.begin(), passports.end(), [](const auto &passport) { return passport.IsValid(); });
        std::cout << "Number of valid passports: " << numberOfValidPassports << "\n";
    }

    return 0;
}
