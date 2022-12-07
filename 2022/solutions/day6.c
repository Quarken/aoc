#include "aoc.h"

static inline int solve(char* input, int input_length, int slice)
{
    uint64_t set = 0;
    for (int i = 0; i < input_length-slice; i++)
    {
        set |= 1ULL << (input[i] - 'A');
        for (int j = 0; j < slice-1; j++)
        {
            char c = input[i+j+1];
            uint64_t current = 1ULL << (c - 'A');
            if (current & set)
            {
                set = 0;
                break;
            }
            else
            {
                set |= current;
            }
        }

        if (set != 0)
        {
            return i + slice;
        }
    }
    return 0;
}

AOC_SOLUTION(6)(char* input, int input_length)
{
    int part1 = solve(input, input_length, 4);
    int part2 = solve(input, input_length, 14);

    return ANSWER(part1, part2);
}
