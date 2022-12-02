#include "aoc.h"

AOC_SOLUTION(2)(char* input)
{
    int part1 = 0, part2 = 0;
    for (char* p = input; *p != 0; p += 4)
    {
        char a = p[0] - 'A';
        char b = p[2] - 'X';
        char diff = a - b;
        char s = a + 3;
        part1 += 1 + b + (diff == 0 ? 3 : 0) + (diff == -1 || diff == 2 ? 6 : 0);
        part2 += 1 + b * 3 + (b == 0 ? (s-1)%3 : 0) + (b == 1 ? a : 0) + (b == 2 ? (s+1)%3 : 0);
        if (p[3] == 0) break;
    }
    return ANSWER(part1, part2);
}