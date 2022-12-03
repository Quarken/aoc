#include "aoc.h"

static int priority(char c)
{
    if (c <= 'Z')
    {
        return c - 'A' + 27;
    }
    else
    {
        return c - 'a' + 1;
    }
}

static uint64_t string_to_bitmask(char* string, int length)
{
    uint64_t result = 0;
    for (int i = 0; i < length; i++)
    {
        result |= (uint64_t)1 << (string[i] - 'A');
    }
    return result;
}

static char bitmask_to_character(uint64_t mask)
{
    int i;
    for (i = 0; i < 64; i++)
    {
        if ((mask >> i) & 1)
        {
            break;
        }
    }
    return i + 'A';
}

AOC_SOLUTION(3)(char* input, int input_length)
{
    int part1 = 0;
    int part2 = 0;
    int index = 0;
    while (index < input_length)
    {
        uint64_t mask = ~0;
        for (int line = 0; line < 3; line++)
        {
            char* ptr = input + index;
            int length = line_length(ptr);
            int len_half = length / 2;

            uint64_t mask1 = string_to_bitmask(ptr, len_half);
            uint64_t mask2 = string_to_bitmask(ptr + len_half, len_half);
            mask &= mask1 | mask2;

            char c = bitmask_to_character(mask1 & mask2);
            part1 += priority(c);
            index += length + 1;
        }
        char c = bitmask_to_character(mask);
        part2 += priority(c);
    }
    return ANSWER(part1, part2);
}
