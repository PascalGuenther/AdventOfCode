#if !defined(AOC_Y2021_PUZZLE_COMMON_HPP)
#define AOC_Y2021_PUZZLE_COMMON_HPP (1)

#define AOC_Y2021_PUZZLE_CLASS_DECLARATION(day)                                                                        \
    class PuzzleDay##day##Impl;                                                                                        \
    class PuzzleDay##day final : public IPuzzle                                                                        \
    {                                                                                                                  \
      public:                                                                                                          \
        PuzzleDay##day(std::string_view input);                                                                        \
        std::optional<std::int64_t> Part1() override;                                                                  \
        std::optional<std::int64_t> Part2() override;                                                                  \
        ~PuzzleDay##day() override;                                                                                    \
                                                                                                                       \
      private:                                                                                                         \
        std::unique_ptr<PuzzleDay##day##Impl> pImpl;                                                                   \
    };

#endif // !defined(AOC_Y2021_PUZZLE_COMMON_HPP)
