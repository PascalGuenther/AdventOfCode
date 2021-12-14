#if !defined(AOC_Y2021_GAME_FACTORY_HPP)
#define AOC_Y2021_GAME_FACTORY_HPP (1)

#include "ipuzzle.hpp"
#include "types.hpp"

#include <cstdint>
#include <memory>
#include <string_view>

namespace AOC::Y2021
{

AOC_Y2021_API
class PuzzleFactory final
{
  public:
    static std::unique_ptr<IPuzzle> CreatePuzzle(std::uint8_t game, std::string_view input);
};

} // namespace AOC::Y2021

#endif // !defined(AOC_Y2021_GAME_FACTORY_HPP)
