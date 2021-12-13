#include "puzzle_factory.hpp"
#include "puzzle_common.hpp"

#define AOC_Y2021_SUPPORTED_PUZZLES_LIST(X_)                                                                           \
    X_(01)                                                                                                             \
    X_(02)                                                                                                             \
    X_(03)                                                                                                             \
    X_(04)                                                                                                             \
    X_(05)                                                                                                             \
    X_(06)                                                                                                             \
    X_(07)

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
    case day:                                                                                                          \
        return std::make_unique<PuzzleDay##day>(input);

        AOC_Y2021_SUPPORTED_PUZZLES_LIST(AOC_Y2021_PUZZLE_CASE)

#undef AOC_Y2021_PUZZLE_CASE
    }
}

} // namespace AOC::Y2021
