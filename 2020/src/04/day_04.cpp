#include <algorithm>
#include <array>
#include <cassert>
#include <fstream>
#include <iostream>
#include <string>
#include <utility>

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
        size_t tokenBeginPos = 0;
        size_t tokenCnt = 0u;
        while (tokenBeginPos < keyValuePairs.size())
        {
            const auto tokenEndPos = keyValuePairs.find_first_of(" ", tokenBeginPos);

            const auto keyValueToken = keyValuePairs.substr(tokenBeginPos, tokenEndPos - tokenBeginPos);

            tokenBeginPos = tokenEndPos + 1u;
            if (keyValueToken.size() < 1u)
            {
                continue;
            }
            else
            {
                tokenCnt++;
            }

            constexpr char delimiter = ':';
            const auto delimiterPos = keyValueToken.find_first_of(delimiter);
            std::string_view key = keyValueToken.substr(0u, delimiterPos);
            std::string_view value = keyValueToken.substr(delimiterPos + 1u);
            auto it = std::find_if(std::begin(aFieldName), std::end(aFieldName),
                                   [&key](const char *name) { return (0 == key.compare(name)); });
            if (it != std::end(aFieldName))
            {
                Field fieldId = static_cast<Field>(std::distance(aFieldName, it));
                if (fieldId < m_aFields.size())
                {
                    m_aFields[fieldId] = value;
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
            if ((tokenCnt >= Field::Max) || (tokenEndPos == std::string_view::npos))
            {
                break;
            }
        }
    }

    [[nodiscard]] bool AreMandatoryFieldsPresent() const
    {
        for (size_t fieldId = 0u; fieldId < Field::Max; fieldId++)
        {
            if (fieldId == Field::cid)
            {
                // ignore this field
                continue;
            }
            if (m_aFields[fieldId].size() < 1)
            {
                // mandatory entry is empty
                return false;
            }
        }
        return true;
    }

    [[nodiscard]] bool AreMandatoryFieldsValid() const
    {
        if (!this->AreMandatoryFieldsPresent())
        {
            return false;
        }
        for (size_t fieldId = 0u; fieldId < m_aFields.size(); ++fieldId)
        {
            const Field eFieldId = static_cast<Field>(fieldId);
            const auto isNumber = [](const std::string_view &s) {
                return ((s.size() > 0u) && (std::all_of(s.begin(), s.end(), [](const auto &c) { return std::isdigit(c); })));
            };
            struct Boundaries
            {
                int lower;
                int upper;
            };
            const auto isWithinBoundaries = [isNumber](const std::string_view &s, const Boundaries &boundaries) {
                if (!isNumber(s))
                {
                    return false;
                }
                const std::string tempString(s);
                const auto value = std::stoi(tempString);
                return ((value >= boundaries.lower) && (value <= boundaries.upper));
            };
            switch (eFieldId)
            {
                case Field::byr:
                    [[fallthrough]];
                case Field::iyr:
                    [[fallthrough]];
                case Field::eyr:
                {
                    if ((4u != m_aFields[eFieldId].size()) || (!isNumber(m_aFields[eFieldId])))
                    {
                        return false;
                    }
                    Boundaries boundaries;
                    switch (eFieldId)
                    {
                        case Field::byr:
                            boundaries = {1920, 2002};
                            break;
                        case Field::iyr:
                            boundaries = {2010, 2020};
                            break;
                        case Field::eyr:
                            boundaries = {2020, 2030};
                            break;

                        default:
                            assert(false);
                            return false;
                    }
                    if (!isWithinBoundaries(m_aFields[eFieldId], boundaries))
                    {
                        return false;
                    }
                    break;
                }

                case Field::hgt:
                {
                    const size_t itemSize = m_aFields[Field::hgt].size();
                    if ((itemSize < 3u) || (itemSize > 5u))
                    {
                        return false;
                    }
                    const auto value{std::string_view{m_aFields[Field::hgt]}.substr(0, itemSize - 2u)};
                    const auto unit{std::string_view{m_aFields[Field::hgt]}.substr(itemSize - 2u)};
                    if (!isNumber(value))
                    {
                        return false;
                    }
                    Boundaries boundaries{};
                    if (0u == unit.compare("cm"))
                    {
                        boundaries = {150, 193};
                    }
                    else if (0u == unit.compare("in"))
                    {
                        boundaries = {59, 76};
                    }
                    else
                    {
                        // unknown unit
                        return false;
                    }
                    if (!isWithinBoundaries(value, boundaries))
                    {
                        return false;
                    }
                    break;
                }

                case Field::hcl:
                {
                    if ((m_aFields[Field::hcl].size() != 7u) || (m_aFields[Field::hcl][0u] != '#'))
                    {
                        return false;
                    }
                    const auto hexValue{std::string_view{m_aFields[Field::hcl]}.substr(1u)};
                    if (hexValue.size() != 6u)
                    {
                        return false;
                    }
                    const bool allLowerCaseHexDegits = std::all_of(hexValue.begin(), hexValue.end(), [](const char c) {
                        const bool isDigit = c >= '0' && c <= '9';
                        const bool isLowerCaseHexDigit = c >= 'a' && c <= 'f';
                        return (isDigit || isLowerCaseHexDigit);
                    });
                    if (!allLowerCaseHexDegits)
                    {
                        return false;
                    }
                    break;
                }
                case Field::ecl:
                {
                    constexpr const char *aValidEyeColors[] = {"amb", "blu", "brn", "gry", "grn", "hzl", "oth"};
                    const bool isColorValid =
                        std::any_of(std::begin(aValidEyeColors), std::end(aValidEyeColors),
                                    [this](const char *color) { return (0u == this->m_aFields[Field::ecl].compare(color)); });
                    if (!isColorValid)
                    {
                        return false;
                    }
                    break;
                }
                case Field::pid:
                {
                    if ((m_aFields[Field::pid].size() != 9u) || (!isNumber(m_aFields[Field::pid])))
                    {
                        return false;
                    }
                    break;
                }
                case Field::cid:
                {
                    continue;
                }

                default:
                    assert(false);
                    return false;
            }
        }
        return true;
    }

private:
    std::array<std::string, Field::Max> m_aFields;
};

[[nodiscard]] int ReadPassportFile(const char *filename)
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
    size_t mandatoryPresentCount = 0u;
    size_t validPassportsCount = 0u;
    const auto processRecord = [&mandatoryPresentCount, &validPassportsCount](std::string &record) {
        if (!record.empty())
        {
            Passport passport(record);
            record = "";
            if (passport.AreMandatoryFieldsPresent())
            {
                mandatoryPresentCount++;
            }
            if (passport.AreMandatoryFieldsValid())
            {
                validPassportsCount++;
            }
        }
    };
    std::string record;
    std::string line;
    while (std::getline(inputFile, line))
    {
        if (line.empty())
        {
            processRecord(record);
        }
        else
        {
            record += " " + line;
        }
    }
    processRecord(record);
    {
        std::cout << "=Part 1=\n";
        std::cout << "All mandatory fields valid in " << mandatoryPresentCount << " passports\n";
    }
    {
        std::cout << "=Part 2=\n";
        std::cout << "Valid passports: " << validPassportsCount << "\n";
    }
    return 0;
}

int main(int argc, const char *argv[])
{
    std::cout << "==Day 04==\n";
    if (argc < 2)
    {
        std::cout << "Please specify an input file\n";
        return 1;
    }
    const int ret = ReadPassportFile(argv[1u]);
    if (0 != ret)
    {
        std::cerr << "Failed to read passports from file\n";
        return 1;
    }

    return ret;
}
