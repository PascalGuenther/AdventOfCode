#include <fstream>
#include <iostream>
#include <optional>
#include <string>
#include <vector>

using std::size_t;

enum class MapItem
{
    Tree,
    Open,
    Max
};

constexpr char MapItemCharTree = '#';
constexpr char MapItemCharOpen = '.';

constexpr char MapItemToChar(MapItem item)
{
    switch (item)
    {
        case MapItem::Tree:
            return MapItemCharTree;
        case MapItem::Open:
            return MapItemCharOpen;
        case MapItem::Max:
            [[fallthrough]];
        default:
            throw std::runtime_error("Invalid Item");
            break;
    }
}

constexpr MapItem MapItemFromChar(const char c)
{
    switch (c)
    {
        case MapItemCharTree:
            return MapItem::Tree;
        case MapItemCharOpen:
            return MapItem::Open;
        default:
            throw std::runtime_error("Invalid character");
            break;
    }
}

class Terrain
{
public:
    using row_t = uint32_t;

public:
    explicit Terrain(size_t width) : m_width(width)
    {
        if (m_width > (8 * sizeof(row_t)))
        {
            throw std::runtime_error("Map too wide");
        }
    }

    explicit Terrain(const std::string &filename)
    {
        std::ifstream inputFile(filename.c_str(), std::ifstream::in);
        if (!inputFile.good())
        {
            throw std::runtime_error("Map too wide");
        }
        std::string line;
        bool isFirstLine = true;
        while (std::getline(inputFile, line))
        {
            if (isFirstLine)
            {
                m_width = line.size();
                isFirstLine = false;
            }
            this->AddRow(line);
        }
        inputFile.close();
    }

    bool AddRow(const std::string &geography)
    {
        if (geography.size() > (8 * sizeof(row_t)))
        {
            throw std::runtime_error("invalid width");
        }
        row_t newRow{};
        size_t position = 0u;
        for (const char &c : geography)
        {
            MapItem item = MapItemFromChar(c);
            if (item == MapItem::Tree)
            {
                newRow |= (1u << position);
            }
            position++;
        }
        m_rows.push_back(newRow);
        return true;
    }

    [[nodiscard]] size_t GetHeight() const { return m_rows.size(); }

    [[nodiscard]] MapItem GetItem(size_t xPos, size_t yPos) const
    {
        if (yPos >= m_rows.size())
        {
            throw std::runtime_error("invalid position");
        }
        xPos %= m_width;
        return (m_rows[yPos] & (1u << xPos)) ? MapItem::Tree : MapItem::Open;
    }

private:
    std::vector<row_t> m_rows;
    size_t m_width{};
};

int main(int argc, const char *argv[])
{
    std::cout << "==Day 03==\n";
    if (argc < 2)
    {
        std::cout << "Please specify an input file\n";
        return 1;
    }
    try
    {
        const Terrain areaMap = Terrain(argv[1u]);
        {
            std::cout << "=Part 1=\n";
            size_t xPos = 0u;
            size_t yPos = 0u;
            size_t treeCount = 0u;
            const auto yMax = areaMap.GetHeight();
            while (yPos < yMax)
            {
                if (MapItem::Tree == areaMap.GetItem(xPos, yPos))
                {
                    treeCount++;
                }
                xPos += 3u;
                yPos += 1u;
            }
            std::cout << "We have encountered " << treeCount << " trees.\n";
        }
    }
    catch (std::exception & /*e*/)
    {
        std::cout << "Failed to parse input";
        return 1;
    }

    return 0;
}