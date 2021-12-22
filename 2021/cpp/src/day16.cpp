#include "ipuzzle.hpp"
#include "puzzle_common.hpp"
#include "utils.hpp"

#include <algorithm>
#include <array>
#include <cstddef>
#include <cstdint>
#include <memory>
#include <numeric>
#include <optional>
#include <ranges>
#include <string_view>
#include <utility>
#include <variant>
#include <vector>

namespace AOC::Y2021
{

namespace Day16
{

namespace
{

class BinaryStream
{
  public:
    AOC_Y2021_CONSTEXPR BinaryStream(std::size_t nibbles, std::unique_ptr<std::uint8_t[]> binary)
        : nibbles(nibbles), binary(std::move(binary))
    {
    }

    AOC_Y2021_CONSTEXPR std::uint8_t ReadBits(std::uint32_t startBit, std::uint32_t bitCount) const
    {
        if (bitCount > 8U)
        {
            return 0U;
        }
        if (((startBit + bitCount + 3U) / 4U) > nibbles)
        {
            return 0U;
        }
        const std::size_t beginByte = startBit / 8U;
        const std::size_t beginBit = startBit % 8U;
        std::uint8_t result = binary[beginByte] << (beginBit + 8U - bitCount);
        const std::uint8_t extractedBitsStep1 = (8U - beginBit);
        if (extractedBitsStep1 < bitCount)
        {
            const std::size_t remainingBits = bitCount - extractedBitsStep1;
            result = result | (binary[beginByte] >> (8U - remainingBits));
        }
        return result;
    }

    AOC_Y2021_CONSTEXPR std::size_t GetNibblesCnt() const
    {
        return nibbles;
    }

    constexpr static std::size_t GetSizeBits()
    {
        return 6U;
    }

  private:
    std::size_t nibbles;
    std::unique_ptr<std::uint8_t[]> binary;
};

AOC_Y2021_CONSTEXPR BinaryStream ReadInput(std::string_view input)
{
    std::size_t sizeBytes = (input.size() + 1u) / 2u;
    if (sizeBytes == 0)
    {
        return BinaryStream{0, nullptr};
    }
    auto pBinary = std::make_unique<std::uint8_t[]>(sizeBytes);
    std::size_t nibbles;
    for (const auto &c : input)
    {
        std::uint8_t val4bit;
        if ((c == '\r') || (c == '\n'))
        {
            break;
        }
        else if ((c >= '0') && (c <= '9'))
        {
            val4bit = c - '0';
        }
        else if ((c >= 'A') && (c <= 'F'))
        {
            val4bit = c - 'A' + 10U;
        }
        else
        {
            return BinaryStream(0U, nullptr);
        }
        pBinary[nibbles / 2U] |= val4bit << (((nibbles % 2) == 0) ? 4U : 0U);
        ++nibbles;
    }
    return BinaryStream(nibbles, std::move(pBinary));
}

struct PacketHeader
{
    enum class PacketType : std::uint8_t
    {
        A = 0b000,
        B = 0b001,
        C = 0b010,
        D = 0b011,
        Literal = 0b100,
        E = 0b101,
        F = 0b110,
        G = 0b111,
    };
    AOC_Y2021_CONSTEXPR PacketHeader(std::uint8_t val)
        : version((val & 0b0011'1000) >> 3U), type(static_cast<PacketType>(val & 0b111))
    {
    }
    std::uint8_t version : 3U;
    PacketType type : 3U;
};

struct Literal
{
    AOC_Y2021_CONSTEXPR Literal(std::uint8_t val) : val(val){};
    std::uint8_t val : 4u;
};

struct Operator
{
};

using PacketItem = std::variant<PacketHeader, Literal, Operator>;

AOC_Y2021_CONSTEXPR std::vector<PacketItem> ParseBinary(const BinaryStream &binary)
{
    std::vector<PacketItem> parsed;
    std::size_t maxBits = 4U * binary.GetNibblesCnt();
    std::size_t bitPos = 0u;
    while (bitPos < maxBits)
    {
        {
            constexpr auto bitCnt = PacketHeader::GetSizeBits();
            const std::uint8_t bits = binary.ReadBits(bitPos, bitCnt);
            parsed.push_back(PacketHeader(bits));
            bitPos += bitCnt;
        }
        {
            const auto &header = parsed.back();
            switch (header.type)
            {
            case PacketType::PacketType::Literal: {
                while (true)
                {

                    constexpr auto bitCnt = 5U;
                    const std::uint8_t bits = binary.ReadBits(bitPos, bitCnt);
                    parsed.push_back(Literak(bits & 0b1111));
                    bitPos += bitCnt;
                    bool moreData = (bits & 0b1'000) != 0;
                    if (!moreData)
                    {
                        break;
                    }
                }
                break;
            }
            default:
                break;
            }
        }
    }
    return parsed;
}

#if AOC_Y2021_CONSTEXPR_UNIT_TEST
[[maybe_unused]] consteval void Test()
{
    constexpr std::string_view exampleInput = "NNCB\n\nCH -> B\nHH -> N\nCB -> H\nNH -> C\nHB -> C\nHC -> B\r\nHN -> "
                                              "C\nNN -> C\nBH -> H\nNC -> B\nNB -> B\nBN "
                                              "-> B\r\nBB -> N\nBC -> B\nCC -> N\nCN -> C";
    static_assert(std::get<std::int64_t>(GrowPolymer(ParseInput(exampleInput), 10)) == 1588,
                  "Day 16 part 1: error calculating resut");
    static_assert(std::get<std::int64_t>(GrowPolymer(ParseInput(exampleInput), 40)) == 2'188'189'693'529,
                  "Day 16 part 2: error calculating resut");
}

#endif // AOC_Y2021_CONSTEXPR_UNIT_TEST

} // namespace

} // namespace Day16

#if defined(AOC_Y2021_PUZZLE_CLASS_DECLARATION)

AOC_Y2021_PUZZLE_CLASS_DECLARATION(16)

class PuzzleDay16Impl final
{
  public:
    PuzzleDay16Impl(const std::string_view &input) : binary(Day16::ReadInput(input))
    {
    }

    const auto &Get() const
    {
        return binary;
    }

  private:
    Day16::BinaryStream binary;
};

PuzzleDay16::PuzzleDay16(const std::string_view input) : pImpl(std::make_unique<PuzzleDay16Impl>(input))
{
}
PuzzleDay16::~PuzzleDay16() = default;

[[nodiscard]] IPuzzle::Solution_t PuzzleDay16::Part1()
{
    if (!pImpl)
    {
        return std::monostate{};
    }
    return std::monostate{};
}

[[nodiscard]] IPuzzle::Solution_t PuzzleDay16::Part2()
{
    if (!pImpl)
    {
        return std::monostate{};
    }
    return std::monostate{};
}

#endif // defined(AOC_Y2021_PUZZLE_CLASS_DECLARATION)

} // namespace AOC::Y2021
