#if defined(_MSC_VER)
#define _CRT_SECURE_NO_DEPRECATE
#include <intrin.h>
#endif
#include <inttypes.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#define AOC_INPUT_MAX_LETTERS  (26)
#define AOC_INPUT_LINEBUF_SIZE (AOC_INPUT_MAX_LETTERS + sizeof((char)'\n') + sizeof((char)'\0'))

typedef struct
{
    uint32_t anyoneYes;
    uint32_t everyoneYes;
} GroupDeclaration_t;

static bool ParseSingleDeclaration(const char *pLine, uint32_t *const pDeclaration)
{
    if ((NULL == pLine) || (NULL == pDeclaration))
    {
        return false;
    }

    *pDeclaration = 0u;
    for (size_t i = 0u; i < AOC_INPUT_MAX_LETTERS; ++i)
    {
        const char c = pLine[i];
        switch (c)
        {
            case '\n':
            case '\r':
            case '\0':
                return true;

            default:
                break;
        }

        if ((pLine[i] < 'a') || (pLine[i] > 'z'))
        {
            // invalid character
            return false;
        }

        const uint8_t answerIndex = pLine[i] - 'a';
        *pDeclaration |= 1ul << answerIndex;
    }
    return true;
}

static uint32_t CountAnswerYes(const uint32_t declaration)
{
#if defined(_MSC_VER)
    return __popcnt(declaration);
#elif defined(__GNUC__) || defined(__GNUG__)
    return __builtin_popcount(declaration);
#else
    uint32_t ret = 0u;
    for (size_t i = 0u; i < 8 * sizeof(declaration); ++i)
    {
        if (declaration & (1u << i))
        {
            ret++;
        }
    }
    return ret;
#endif
}

static bool LoadCustomsDeclarationsFromFile(const char *const pFilename, GroupDeclaration_t *const pSumOfGroupDeclarations)
{
    if ((NULL == pFilename) || (NULL == pSumOfGroupDeclarations))
    {
        return false;
    }

    FILE *const pFile = fopen(pFilename, "r");
    if (NULL == pFile)
    {
        puts("Failed to open input file");
        return false;
    }

    bool endOfFile = false;
    do
    {
        GroupDeclaration_t declarationsOfGroup = {
            .anyoneYes = 0u,
            // set flag to "yes" for all possible answers, will be reset if anyone answers "no"
            .everyoneYes = (1ul << AOC_INPUT_MAX_LETTERS) - 1u,
        };
        while (true)
        {
            char aLineBuf[AOC_INPUT_LINEBUF_SIZE] = {0u};
            if (!fgets(aLineBuf, sizeof(aLineBuf), pFile))
            {
                endOfFile = true;
                break;
            }

            if (aLineBuf[0] == '\n')
            {
                break;
            }

            uint32_t individualDeclaration = 0u;
            if (!ParseSingleDeclaration(&aLineBuf[0u], &individualDeclaration))
            {
                fclose(pFile);
                return false;
            }
            // Part 1
            declarationsOfGroup.anyoneYes |= individualDeclaration;
            // Part 2
            // reset the group's bits corresponding to the questions the person didn't answer "yes"
            declarationsOfGroup.everyoneYes &= individualDeclaration;
        }
        pSumOfGroupDeclarations->anyoneYes += CountAnswerYes(declarationsOfGroup.anyoneYes);
        pSumOfGroupDeclarations->everyoneYes += CountAnswerYes(declarationsOfGroup.everyoneYes);
    } while (!endOfFile);
    fclose(pFile);
    return true;
}

int main(const int argc, const char *argv[])
{
    puts("==Day 06==");
    if (argc < 2)
    {
        puts("Please specify an input file");
        return 1;
    }

    GroupDeclaration_t sumsOfAnswerYesInGroupDeclarations = {0u};
    if (!LoadCustomsDeclarationsFromFile(argv[1u], &sumsOfAnswerYesInGroupDeclarations))
    {
        puts("Failed to open or parse input file");
        return 1;
    }

    {
        puts("=Part 1=");
        printf("Sum of \"anybody in group answered yes\" counts: %" PRIu32 "\n", sumsOfAnswerYesInGroupDeclarations.anyoneYes);
    }
    {
        puts("=Part 2=");
        printf("Sum of \"everyone in group answered yes\" counts: %" PRIu32 "\n", sumsOfAnswerYesInGroupDeclarations.everyoneYes);
    }

    return 0;
}
