#include "aoc.h"
#include <stdlib.h>

AOC_SOLUTION(1)(char* input)
{
    int running_max[4] = {0};
    char* ptr = input;
    int current_elf_sum = 0;
    while (1)
    {
        int num = strtol(ptr, &ptr, 10);
        current_elf_sum += num;
        if (*ptr == 0 || *(ptr+1) == '\n')
        {
            for (int i = 0; i < 3; i++)
            {
                if (current_elf_sum >= running_max[i])
                {
                    memmove(running_max + i + 1, running_max + i, 3 - i);
                    running_max[i] = current_elf_sum;
                    break;
                }
            }
            current_elf_sum = 0;
        }

        if (*ptr == 0) break;
    }

    return ANSWER(
        running_max[0],
        running_max[0] + running_max[1] + running_max[2]
    );
}