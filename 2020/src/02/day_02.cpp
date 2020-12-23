#include "input_2020_02_a.h"

#include <algorithm>
#include <array>
#include <charconv>
#include <iostream>
#include <string>
#include <utility>
#include <vector>


using std::size_t;
using string_t = std::string_view;

constexpr std::pair<const string_t, const string_t> SplitString(const string_t &input, const string_t &delimiter)
{
    const auto position = input.find_first_of(delimiter);
    return std::make_pair(input.substr(0, position), input.substr(position + 1u));
}

struct PasswordPolicy
{
    explicit PasswordPolicy(const string_t policyString)
    {
        const auto [boundaries, specialLetterString] = SplitString(policyString, " ");
        m_specialLetter = specialLetterString[0];
        const auto [minString, maxString] = SplitString(policyString, "-");
        auto [pMin, ecMin] = std::from_chars(minString.data(), minString.data() + minString.size(), m_minOccurrence);
        auto [pMax, ecMax] = std::from_chars(maxString.data(), maxString.data() + maxString.size(), m_maxOccurrence);
        if ((ecMin != std::errc()) || (ecMax != std::errc()))
        {
            throw std::invalid_argument{"invalid_argument"};
        }
    }
    char m_specialLetter;
    size_t m_minOccurrence;
    size_t m_maxOccurrence;
};

struct PasswordListEntry
{
    PasswordListEntry(const string_t &policyString, const string_t &password) : m_policy(policyString), m_password(password) {}
    [[nodiscard]] bool IsValid() const
    {
        if (m_password.size() < 1u)
        {
            return false;
        }
        const size_t occurrences = std::count(m_password.begin(), m_password.end(), m_policy.m_specialLetter);
        return ((occurrences >= m_policy.m_minOccurrence) && (occurrences <= m_policy.m_maxOccurrence));
    }
    const PasswordPolicy m_policy;
    const string_t m_password;
};

int main()
{
    std::cout << "==Day 02==\n";

    constexpr string_t aInput[] = AOC_INPUT_2020_02_A;
    std::vector<PasswordListEntry> passwordList;
    passwordList.reserve(sizeof(aInput) / sizeof(*aInput));
    for (const auto &line : aInput)
    {
        const auto splitLine = SplitString(line, ":");
        passwordList.emplace_back(splitLine.first, splitLine.second);
    }

    {
        std::cout << "=Part 1=\n";
        const auto numOfValidPasswords =
            std::count_if(passwordList.begin(), passwordList.end(), [](const auto &entry) { return entry.IsValid(); });
        std::cout << "Number of valid passwords: " << numOfValidPasswords << "\n";
    }

    return 0;
}
