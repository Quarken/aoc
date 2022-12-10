#include "aoc.h"
#include <stdlib.h>

AOC_SOLUTION(10)(char* input, int input_length)
{
    int cycle = 0;
    int reg = 1;


    int part1 = 0;
    int width = 40;
    int height = 6;
    char* crt = alloc_array(width * height, char);

    string_split_result lines = split_by((string){input, input_length}, '\n');
    for (int line_index = 0; line_index < lines.count; line_index++)
    {
        int reg0 = reg;
        int cycle0 = cycle;

        string line = lines.strings[line_index];
        switch (line.data[0])
        {
            case 'n':
            {
                cycle += 1;
            } break;

            case 'a':
            {
                cycle += 2;
                int len = sizeof("addx ") - 1;
                int add = parse_int((string){line.data + len, line.length - len});
                reg += add;
            } break;
        }

        for (int i = cycle0; i < cycle; i++)
        {
            int pos = i%40;
            crt[i] = (pos >= reg0-1 && pos <= reg0+1) ? '#' : '.';
        }

        int part1_values[] = { 20, 60, 100, 140, 180, 220 };
        for (int i = 0; i < ARRAY_SIZE(part1_values); i++)
        {
            int value = part1_values[i];
            if (cycle0 < value && cycle >= value)
            {
                part1 += reg0 * value;
            }
        }
    }

    char* part2 = alloc_array((width+1) * height, char);
    for (int i = 0; i < height; i++)
    {
        memcpy(part2 + i*(width+1), crt + i*width, width);
        part2[i * (width+1) + 40] = '\n';
    }
    part2[height * (width+1) + width] = 0;
    return ANSWER(part1, part2);
}