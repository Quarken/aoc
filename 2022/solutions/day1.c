#include "aoc.h"

int parse_int(char* ptr, char** end) {
    int result = 0;
    for (; *ptr != 0 && *ptr != '\n'; ++ptr)
    {
        result = 10 * result + (*ptr) - '0';
    }
    *end = ptr;
    return result;
}

AOC_SOLUTION(1)(char* input)
{
    int running_max[4] = {0};
    int current_elf_sum = 0;

    char *ptr = input;
    while (1)
    {
        int num = parse_int(ptr, &ptr);
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

        if (*ptr == 0) break;
        ++ptr;
    }

    return ANSWER(
        running_max[0],
        running_max[0] + running_max[1] + running_max[2]
    );
}