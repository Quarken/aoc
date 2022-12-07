#include "aoc.h"

enum
{
    ROCK = 1,
    PAPER = 2,
    SCISSORS = 3
};

enum
{
    LOSE = 1,
    DRAW = 2,
    WIN = 3
};

AOC_SOLUTION(2)(char* input, int input_length)
{
    int lookup_part1[4][4] = {
        [ROCK] = {
            [ROCK]     = 1 + 3,
            [PAPER]    = 2 + 6,
            [SCISSORS] = 3 + 0
        },
        [PAPER] = {
            [ROCK]     = 1 + 0,
            [PAPER]    = 2 + 3,
            [SCISSORS] = 3 + 6
        },
        [SCISSORS] = {
            [ROCK]     = 1 + 6,
            [PAPER]    = 2 + 0,
            [SCISSORS] = 3 + 3
        },
    };

    int lookup_part2[4][4] = {
        [ROCK] = {
            [LOSE] = 0 + SCISSORS,
            [DRAW] = 3 + ROCK,
            [WIN]  = 6 + PAPER
        },
        [PAPER] = {
            [LOSE] = 0 + ROCK,
            [DRAW] = 3 + PAPER,
            [WIN]  = 6 + SCISSORS,
        },
        [SCISSORS] = {
            [LOSE] = 0 + PAPER,
            [DRAW] = 3 + SCISSORS,
            [WIN]  = 6 + ROCK
        },
    };

    int part1 = 0, part2 = 0;
    for (char* p = input; *p != 0; p += 4)
    {
        char a = p[0] - 'A' + 1;
        char b = p[2] - 'X' + 1;
        part1 += lookup_part1[(int)a][(int)b];
        part2 += lookup_part2[(int)a][(int)b];
        if (p[3] == 0) break;
    }
    return ANSWER(part1, part2);
}