#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <numeric>
#include <stdexcept>
#include <string>
#include <string_view>
#include <vector>
using std::size_t;

static constexpr size_t maxRecursionDepth{30u};

namespace
{

static constexpr std::string_view aExampleInput[] = {
    "light red bags contain 1 bright white bag, 2 muted yellow bags.",
    "dark orange bags contain 3 bright white bags, 4 muted yellow bags.",
    "bright white bags contain 1 shiny gold bag.",
    "muted yellow bags contain 2 shiny gold bags, 9 faded blue bags.",
    "shiny gold bags contain 1 dark olive bag, 2 vibrant plum bags.",
    "dark olive bags contain 3 faded blue bags, 4 dotted black bags.",
    "vibrant plum bags contain 5 faded blue bags, 6 dotted black bags.",
    "faded blue bags contain no other bags.",
    "dotted black bags contain no other bags.",
};

class Bag
{
private:
    static constexpr const char firstSeparator[] = " bags contain ";

public:
    explicit Bag(std::string_view ruleAsText) : m_color(ParseOuterBagColor(ruleAsText))
    {
        constexpr const char noContainedBagsIdentifier[] = " bags contain no other bags";
        const bool doesContainOtherBags = (std::string_view::npos == ruleAsText.find(noContainedBagsIdentifier));
        if (!doesContainOtherBags)
        {
            return;
        }

        const auto outerBagColorEndPos = ruleAsText.find(firstSeparator);
        if (std::string_view::npos == outerBagColorEndPos)
        {
            throw("Parsing failed");
        }
        auto quantityStringBeginPos = outerBagColorEndPos + sizeof(firstSeparator) - sizeof(static_cast<char>('\0'));
        do
        {
            const auto quantityStringEndPos = ruleAsText.find_first_of(" ", quantityStringBeginPos);
            if ((std::string_view::npos == quantityStringEndPos) || (quantityStringBeginPos >= quantityStringEndPos))
            {
                throw("Failed to parse quantity");
            }
            const std::string quantityString(
                ruleAsText.substr(quantityStringBeginPos, quantityStringEndPos - quantityStringBeginPos));
            const size_t quantity = std::stoul(quantityString);
            constexpr const char colorStringEnding[] = " bag";
            const auto colorStringBeginPos = quantityStringEndPos + sizeof(static_cast<char>(' '));
            const auto colorStringEndPos = ruleAsText.find(colorStringEnding, colorStringBeginPos);
            if ((std::string_view::npos == colorStringEndPos) || (colorStringBeginPos >= colorStringEndPos))
            {
                throw("Failed to parse color");
            }
            const std::string_view color = ruleAsText.substr(colorStringBeginPos, colorStringEndPos - colorStringBeginPos);
            if (!(m_containedBags.insert({std::string(color), quantity}).second))
            {
                throw("Failed to insert quantity of contained bag type");
            }
            constexpr const char bagSeparator[] = ", ";
            quantityStringBeginPos = ruleAsText.find(bagSeparator, colorStringEndPos);
            if (std::string_view::npos == quantityStringBeginPos)
            {
                break;
            }
            quantityStringBeginPos += sizeof(bagSeparator) - sizeof(static_cast<char>('\0'));
        } while (quantityStringBeginPos < ruleAsText.size());
        return;
    }

    [[nodiscard]] size_t ContainedQuantityOfColor(const std::vector<Bag> &others, const std::string_view &color,
                                                  size_t recursionDepth = 0u) const
    {
        if (recursionDepth > maxRecursionDepth)
        {
            throw std::runtime_error("Too many recursions");
        }
        if (0 == color.compare(m_color))
        {
            return 0u;
        }
        const auto it = m_containedBags.find(std::string(color));
        const size_t containedByUs = (m_containedBags.end() == it) ? 0u : it->second;

        const size_t containedRecursively = std::accumulate(
            m_containedBags.begin(), m_containedBags.end(), containedByUs,
            [&others, &color, recursionDepth](auto acc, const auto &containedBag) {
                const auto &color_ = containedBag.first;
                const auto &quantity = containedBag.second;
                const auto it = std::find_if(others.begin(), others.end(), [&color_](const auto &other) {
                    return other.DoesMatchColor(color_);
                });
                if (it == others.end())
                {
                    throw std::runtime_error("Bag not found");
                }
                acc += quantity * it->ContainedQuantityOfColor(others, color, recursionDepth + 1u);
                return acc;
            });
        return containedRecursively;
    }

    [[nodiscard]] bool DoesMatchColor(const std::string_view &color) const { return (m_color == color); }

    [[nodiscard]] size_t CountNestedBags(const std::vector<Bag> &others, size_t recursionDepth = 0u) const
    {
        if (recursionDepth > maxRecursionDepth)
        {
            throw std::runtime_error("Too many recursions");
        }
        const size_t initalAcc{0u};
        const size_t containedRecursively = std::accumulate(
            m_containedBags.begin(), m_containedBags.end(), initalAcc,
            [&others, recursionDepth](auto acc, const auto &containedBag) {
                const auto &color_ = containedBag.first;
                const auto &quantity = containedBag.second;
                const auto it = std::find_if(others.begin(), others.end(), [&color_](const auto &other) {
                    return other.DoesMatchColor(color_);
                });
                if (it == others.end())
                {
                    throw std::runtime_error("Bag not found");
                }
                acc += quantity * (1 + it->CountNestedBags(others, recursionDepth + 1u));
                return acc;
            });
        return containedRecursively;
    }

    [[nodiscard]] static std::string ParseOuterBagColor(const std::string_view &ruleAsText)
    {
        const auto separatorPos = ruleAsText.find(firstSeparator);
        if (std::string_view::npos == separatorPos)
        {
            throw std::runtime_error("Failed to parse input");
        }
        std::string outerBagColor(ruleAsText.substr(0, separatorPos));
        return outerBagColor;
    }

    [[nodiscard]] bool operator<(const Bag &other) const { return (this->m_color < other.m_color); }

private:
    const std::string m_color;
    std::map<std::string, std::size_t> m_containedBags;
};

bool ParseAndAddRule(const std::string_view ruleAsText, std::vector<Bag> &bagsWithRules)
{
    try
    {
        bagsWithRules.emplace_back(ruleAsText);
        return true;
    }
    catch (std::runtime_error &e)
    {
        std::cerr << e.what();
    }
    catch (...)
    {
    }
    return false;
}

[[nodiscard]] int ReadLuggageRulesFromFile(const char *filename, std::vector<Bag> &bagsWithRules)
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
    std::string line;
    while (std::getline(inputFile, line))
    {
        if (line.empty())
        {
            continue;
        }
        if (!ParseAndAddRule(line, bagsWithRules))
        {
            return 1;
        }
    }
    return 0;
}

} // namespace

int main(const int argc, const char *const argv[])
{
    std::cout << "==Day 07==\n";
    std::vector<Bag> bagsWithRules;
    if (argc < 2)
    {
        std::cout << "Using example input. Alternatively, you can pass the path to an input file as command line argument\n";
        for (const auto &line : aExampleInput)
        {
            if (!ParseAndAddRule(line, bagsWithRules))
            {
                return 1;
            }
        }
    }
    else
    {
        const int ret = ReadLuggageRulesFromFile(argv[1u], bagsWithRules);
        if (0 != ret)
        {
            std::cerr << "Failed to read luggage rules from file\n";
            return ret;
        }
    }
    constexpr std::string_view colorOfInterterst = "shiny gold";
    int ret = 0u;
    std::cout << "Input parsing complete" << std::endl;
    {
        std::cout << "=Part 1=\n";
        try
        {
            const auto canAtLeastHoldOneBagOfColor = std::count_if(
                bagsWithRules.begin(), bagsWithRules.end(), [&colorOfInterterst, &bagsWithRules](const auto &bag) -> bool {
                    return (bag.ContainedQuantityOfColor(bagsWithRules, colorOfInterterst) >= 1u);
                });
            std::cout << "Number of bags which can eventually contain at least one " << colorOfInterterst
                      << " bag: " << canAtLeastHoldOneBagOfColor << "\n";
        }
        catch (...)
        {
            std::cerr << "Calculation of part 1 failed";
            ret = 1;
        }
    }
    {
        std::cout << "=Part 2=\n";
        try
        {
            const auto it = std::find_if(bagsWithRules.begin(), bagsWithRules.end(),
                                         [&colorOfInterterst](const auto &bag) { return bag.DoesMatchColor(colorOfInterterst); });
            if (it == bagsWithRules.end())
            {
                throw;
            }
            const auto containedIndividualBags = it->CountNestedBags(bagsWithRules);
            std::cout << "The " << colorOfInterterst << " bag contains " << containedIndividualBags << " individual bags.\n";
        }
        catch (...)
        {
            std::cerr << "Calculation of part 2 failed";
            ret = 1;
        }
    }

    return ret;
}
