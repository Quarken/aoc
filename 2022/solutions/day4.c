#include "aoc.h"

static int parse_int(char* ptr, int* output) {
    int result = 0;
    int length = 0;
    for (; *ptr >= '0' && *ptr <= '9'; ++ptr, ++length)
    {
        result = 10 * result + (*ptr) - '0';
    }
    *output = result;
    return length;
}

AOC_SOLUTION(4)(char* input, int input_length)
{
    int part1 = 0;
    int part2 = 0;
    char *ptr = input;

    int index = 0;
    while (index < input_length)
    {
        int a1, a2, b1, b2;

        index += parse_int(ptr+index, &a1) + 1;
        index += parse_int(ptr+index, &a2) + 1;
        index += parse_int(ptr+index, &b1) + 1;
        index += parse_int(ptr+index, &b2) + 1;

        part1 += (a1 <= b1 && a2 >= b2) || (b1 <= a1 && b2 >= a2);
        part2 += a2 >= b1 && a1 <= b2;
    }
    return ANSWER(part1, part2);
}