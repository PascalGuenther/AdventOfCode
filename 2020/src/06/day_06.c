#if defined(_MSC_VER)
#define _CRT_SECURE_NO_DEPRECATE
#include <intrin.h>
#elif defined(__GNUC__) || defined(__GNUG__)
#else
#endif
#include <inttypes.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#define AOC_INPUT_MAX_LETTERS    (26)
#define AOC_INPUT_LINEBUF_SIZE   (AOC_INPUT_MAX_LETTERS + sizeof((char)'\n') + sizeof((char)'\0'))
#define AOC_MAX_PESSENGER_GROUPS (1024)

static bool AddLine(const char *pLine, uint32_t *const pGroupDeclaration)
{
    if ((NULL == pLine) || (NULL == pGroupDeclaration))
    {
        return false;
    }

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
        *pGroupDeclaration |= 1u << answerIndex;
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

static bool LoadCustomsDeclarationsFromFile(const char *const pFilename, const size_t groupDeclarationsSize,
                                            uint32_t *const pGroupDeclarations)
{
    if ((NULL == pFilename) || (0u == groupDeclarationsSize) || (NULL == pGroupDeclarations))
    {
        return false;
    }
    memset(pGroupDeclarations, 0u, groupDeclarationsSize * sizeof(*pGroupDeclarations));

    FILE *pFile = fopen(pFilename, "r");
    if (NULL == pFile)
    {
        puts("Failed to open input file\n");
        return false;
    }

    char aLineBuf[AOC_INPUT_LINEBUF_SIZE];
    size_t groupIndex = 0u;

    while (fgets(aLineBuf, sizeof(aLineBuf), pFile))
    {
        if (groupIndex >= groupDeclarationsSize)
        {
            puts("Too many passenger groups");
            fclose(pFile);
            return 1;
        }
        if (aLineBuf[0] == '\n')
        {
            groupIndex++;
            continue;
        }
        AddLine(&aLineBuf[0u], &pGroupDeclarations[groupIndex]);
    }
    fclose(pFile);
    return true;
}

int main(const int argc, const char *argv[])
{
    puts("==Day 06==\n");
    if (argc < 2)
    {
        puts("Please specify an input file\n");
        return 1;
    }

    uint32_t aGroupDeclarations[AOC_MAX_PESSENGER_GROUPS] = {0u};
    if (!LoadCustomsDeclarationsFromFile(argv[1u], (sizeof(aGroupDeclarations) / sizeof(*aGroupDeclarations)),
                                         &aGroupDeclarations[0u]))
    {
        puts("Failed to open or parse input file\n");
        return 1;
    }

    {
        puts("=Part 1=\n");
        uint32_t sumOfGroupCounts = 0u;
        for (size_t i = 0u; i < (sizeof(aGroupDeclarations) / sizeof(*aGroupDeclarations)); ++i)
        {
            sumOfGroupCounts += CountAnswerYes(aGroupDeclarations[i]);
        }
        printf("Sum of counts: %" PRIu32 "\n", sumOfGroupCounts);
    }

    return 0;
}
