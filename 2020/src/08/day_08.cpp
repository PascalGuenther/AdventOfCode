#include <fstream>
#include <iostream>
#include <set>
#include <stdexcept>
#include <string>
#include <string_view>
#include <vector>

constexpr const char *aExampleInput[] = {
    "nop +0", "acc +1", "jmp +4", "acc +3", "jmp -3", "acc -9", "acc +1", "jmp -4", "acc +6",
};

#define AOC_OPCODE_LIST(_X_)                                                                                                     \
    _X_(acc)                                                                                                                     \
    _X_(jmp)                                                                                                                     \
    _X_(nop)

namespace
{
enum class Opcode
{
#define AOC_OPCODE_ENUM_ENTRY(entry) entry,
    AOC_OPCODE_LIST(AOC_OPCODE_ENUM_ENTRY) Max,
};

struct Instruction
{
    Instruction(Opcode opcode, int argument) : opcode(opcode), argument(argument) {}
    Opcode opcode;
    const int argument;
};

using Program_t = std::vector<Instruction>;

bool ExecuteProgram(const Program_t &prog, int &accumulator)
{
    std::set<size_t> executedInstructions;
    accumulator = 0;
    size_t programCounter{0};
    do
    {
        try
        {
            const auto &instruction = prog.at(programCounter);
            if (!(executedInstructions.insert(programCounter).second))
            {
                // the instruction at this program counter has been executed before
                return false;
            }

            switch (instruction.opcode)
            {
                case Opcode::jmp:
                    if (instruction.argument < 0)
                    {
                        size_t arg = static_cast<size_t>(-instruction.argument);
                        if (programCounter < arg)
                        {
                            throw std::out_of_range("");
                        }
                        programCounter -= arg;
                        break;
                    }
                    programCounter += instruction.argument;
                    break;
                case Opcode::acc:
                    accumulator += instruction.argument;
                    [[fallthrough]];
                case Opcode::nop:
                    programCounter++;
                    break;

                default:
                    throw std::runtime_error("Unknown instruction");
            }
        }
        catch (const std::out_of_range & /* e */)
        {
            if (programCounter == prog.size())
            {
                // program terminates gracefully by fetching the instruction below the last instruction
                return true;
            }
            throw std::out_of_range("Program counter exceeds code area");
        }

    } while (true);
}

[[nodiscard]] Instruction ParseInstruction(const std::string &instruction)
{
    const Opcode opcode = [&instruction]() -> Opcode {
#define AOC_OPCODE_RETURN(opcode)                                                                                                \
    if (0 == instruction.find(#opcode))                                                                                          \
    {                                                                                                                            \
        return Opcode::opcode;                                                                                                   \
    }
        AOC_OPCODE_LIST(AOC_OPCODE_RETURN)
        throw std::runtime_error("Unknown instruction");
    }();
    const auto argumentBeginPos = instruction.find_first_of(' ');
    return {opcode, std::stoi(instruction.substr(argumentBeginPos))};
}

[[nodiscard]] Program_t LoadProgramFile(const char *const filename)
{
    if (nullptr == filename)
    {
        throw std::runtime_error("Filename is NULL");
    }
    std::ifstream inputFile(filename, std::ifstream::in);
    if (!inputFile.good())
    {
        throw std::runtime_error("Failed to open input file");
    }
    Program_t program;
    std::string line;
    while (std::getline(inputFile, line))
    {
        if (line.empty())
        {
            continue;
        }
        program.push_back(ParseInstruction(line));
    }
    return program;
}

} // namespace

int main(const int argc, const char *const argv[])
{
    std::cout << "==Day 08==\n";
#if !defined(NDEBUG)
    std::cout << "Debug build\n";
#endif
    int ret = 0;
    try
    {
        Program_t program;
        if (argc < 2)
        {
            std::cout << "Using example input. Alternatively, you can pass the path to an input file as command line argument\n";
            program.reserve(sizeof(aExampleInput) / sizeof(*aExampleInput));
            for (const auto &line : aExampleInput)
            {
                program.push_back(ParseInstruction(line));
            }
        }
        else
        {
            program = LoadProgramFile(argv[1u]);
        }

        std::cout << "Input parsing complete" << std::endl;
        {
            std::cout << "=Part 1=\n";
            try
            {
                int accumulator{0};
                if (!ExecuteProgram(program, accumulator))
                {

                    std::cout << "Immediately before any instruction is executed a second time, the value in the accumulator is "
                              << accumulator << ".\n";
                }
                else
                {
                    return 1;
                }
            }
            catch (std::runtime_error &e)
            {
                std::cerr << e.what();
                ret = 1;
            }
            catch (std::out_of_range &e)
            {
                std::cerr << e.what();
                ret = 1;
            }
        }
        {
            std::cout << "=Part 2=\n";
            try
            {
                bool programHasBeenFixed = false;
                for (auto &instruction : program)
                {
                    const Opcode originalOpcode = instruction.opcode;
                    switch (instruction.opcode)
                    {
                        case Opcode::nop:
                            instruction.opcode = Opcode::jmp;
                            break;
                        case Opcode::jmp:
                            instruction.opcode = Opcode::nop;
                            break;
                        default:
                            continue;
                    }
                    int accumulator{0};
                    if (!ExecuteProgram(program, accumulator))
                    {
                        // fix has not worked, revert...
                        instruction.opcode = originalOpcode;
                    }
                    else
                    {
                        std::cout << "After the program terminates, the value in the accumulator is " << accumulator << ".\n";
                        programHasBeenFixed = true;
                        break;
                    }
                }
                if (!programHasBeenFixed)
                {
                    std::cerr << "Failed to fix the program\n";
                    ret = 1;
                }
            }
            catch (std::runtime_error &e)
            {
                std::cerr << e.what();
                ret = 1;
            }
            catch (std::out_of_range &e)
            {
                std::cerr << e.what();
                ret = 1;
            }
        }
    }
    catch (const std::runtime_error &e)
    {
        std::cerr << e.what() << '\n';
        return 1;
    }
    catch (...)
    {
        std::cerr << "Unknown error" << '\n';
        return 1;
    }

    return ret;
}
