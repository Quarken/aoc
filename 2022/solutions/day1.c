#include "aoc.h"

AOC_SOLUTION(1)(char* input, int input_length)
{
    int running_max[4] = {0};
    int current_elf_sum = 0;
    char *ptr = input;
    do
    {
        int num = parse_int_unsafe(ptr, &ptr);
        current_elf_sum += num;
        if (ptr[0] == 0 || ptr[1] == '\n')
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
    } while (*ptr++ != 0);

    return ANSWER(
        running_max[0],
        running_max[0] + running_max[1] + running_max[2]
    );
}