#include "puzzle_factory.hpp"
#include "puzzle_common.hpp"
#include "utils.hpp"

#define AOC_Y2021_SUPPORTED_PUZZLES_LIST(X_)                                                                           \
    X_(01)                                                                                                             \
    X_(02)                                                                                                             \
    X_(03)                                                                                                             \
    X_(04)                                                                                                             \
    X_(05)                                                                                                             \
    X_(06)                                                                                                             \
    X_(07)                                                                                                             \
    X_(08)                                                                                                             \
    X_(09)                                                                                                             \
    X_(10)                                                                                                             \
    X_(11)                                                                                                             \
    X_(12)                                                                                                             \
    X_(13)                                                                                                             \
    X_(14)                                                                                                             \
    X_(15)

namespace AOC::Y2021
{

AOC_Y2021_SUPPORTED_PUZZLES_LIST(AOC_Y2021_PUZZLE_CLASS_DECLARATION)

std::unique_ptr<IPuzzle> PuzzleFactory::CreatePuzzle(const std::uint8_t game, const std::string_view input)
{
    switch (game)
    {

    default:
        return nullptr;

#define AOC_Y2021_PUZZLE_CASE(day)                                                                                     \
    case ParseNumber<std::uint8_t>(#day): /* avoid that leading zeroes are interpreted as octal */                     \
        return std::make_unique<PuzzleDay##day>(input);

        AOC_Y2021_SUPPORTED_PUZZLES_LIST(AOC_Y2021_PUZZLE_CASE)

#undef AOC_Y2021_PUZZLE_CASE
    }
}

} // namespace AOC::Y2021
