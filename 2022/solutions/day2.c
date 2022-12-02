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

AOC_SOLUTION(2)(char* input)
{
    int lookup_part1[4][4] = {
        [ROCK] = {
            [ROCK]     = 1+3,
            [PAPER]    = 2+6,
            [SCISSORS] = 3+0
        },
        [PAPER] = {
            [ROCK]     = 1+0,
            [PAPER]    = 2+3,
            [SCISSORS] = 3+6
        },
        [SCISSORS] = {
            [ROCK]     = 1+6,
            [PAPER]    = 2+0,
            [SCISSORS] = 3+3
        },
    };

    int lookup_part2[4][4] = {
        [ROCK] = {
            [LOSE] = SCISSORS + 0,
            [DRAW] = ROCK     + 3,
            [WIN]  = PAPER    + 6
        },
        [PAPER] = {
            [LOSE] = ROCK     + 0,
            [DRAW] = PAPER    + 3,
            [WIN]  = SCISSORS + 6
        },
        [SCISSORS] = {
            [LOSE] = PAPER    + 0,
            [DRAW] = SCISSORS + 3,
            [WIN]  = ROCK     + 6
        },
    };

    int part1 = 0, part2 = 0;
    for (char* p = input; *p != 0; p += 4)
    {
        char a = p[0] - 'A' + 1;
        char b = p[2] - 'X' + 1;
        part1 += lookup_part1[a][b];
        part2 += lookup_part2[a][b];
        if (p[3] == 0) break;
    }
    return ANSWER(part1, part2);
}