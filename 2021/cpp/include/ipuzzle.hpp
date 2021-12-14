#if !defined(AOC_Y2021_IPUZZLE_HPP)
#define AOC_Y2021_IPUZZLE_HPP (1)

#include "types.hpp"

#include <cstdint>
#include <optional>

namespace AOC::Y2021
{

class IPuzzle
{
  public:
    virtual ~IPuzzle() = default;

    std::optional<std::int64_t> virtual Part1() = 0;
    std::optional<std::int64_t> virtual Part2() = 0;

  protected:
    IPuzzle() = default;
    IPuzzle(const IPuzzle &) = default;
    IPuzzle(IPuzzle &&) = default;
    IPuzzle &operator=(const IPuzzle &) = default;
    IPuzzle &operator=(IPuzzle &&) = default;
};

} // namespace AOC::Y2021

#endif // !defined(AOC_Y2021_IPUZZLE_HPP)
