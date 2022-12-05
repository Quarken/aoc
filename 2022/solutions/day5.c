#include "aoc.h"

#define MAX_STACKS 9

AOC_SOLUTION(5)(char* input, int input_length)
{
    char stacks[MAX_STACKS][255] = {0};
    int counts[MAX_STACKS] = {0};
    char stacks2[MAX_STACKS][255];
    int counts2[MAX_STACKS];

    // split by \n\n first
    int split_point = 0;
    for (int i = 0; i < input_length; i++)
    {
        if (input[i] == '\n' && input[i+1] == '\n')
        {
            split_point = i;
            break;
        }
    }

    string initial_configuration = {
        .data = input,
        .length = split_point
    };
    string all_procedures = { 
        .data = input + split_point,
        .length = input_length - split_point
    };

    string_split_result config = split_by(initial_configuration, '\n');
    string_split_result procedures = split_by(all_procedures, '\n');
    
    int stack_top = config.count - 1;
    for (int i = stack_top-1; i >= 0; i--)
    {
        string str = config.strings[i];
        int stack_index = 0;
        for (int j = 1; j < str.length; j += 4)
        {
            if (str.data[j] != ' ')
            {
                stacks[stack_index][stack_top - i - 1] = str.data[j];
                counts[stack_index]++;
            }
            stack_index++;
        }
    }

    memcpy(stacks2, stacks, sizeof(stacks));
    memcpy(counts2, counts, sizeof(counts));

    for (int i = 0; i < procedures.count; i++)
    {
        string_split_result procedure = split_by(procedures.strings[i], ' ');
        int move = parse_int(procedure.strings[1]);
        int from = parse_int(procedure.strings[3]) - 1;
        int to   = parse_int(procedure.strings[5]) - 1;

        // part 1
        for (int j = 0; j < move; j++)
        {
            int to_index = counts[to];
            int from_index = counts[from] - 1;
            counts[to]++;
            counts[from]--;
            stacks[to][to_index] = stacks[from][from_index];
        }

        // part 2
        int from_index = counts2[from] - move;
        int to_index = counts2[to];
        counts2[to] += move;
        counts2[from] -= move;
        memcpy(&stacks2[to][to_index], &stacks2[from][from_index], move);
    }

    char* part1 = alloc_array(MAX_STACKS+1, char);
    for (int i = 0; i < MAX_STACKS; i++)
    {
        part1[i] = stacks[i][counts[i]-1];
    }

    char* part2 = alloc_array(MAX_STACKS+1, char);
    for (int i = 0; i < MAX_STACKS; i++)
    {
        part2[i] = stacks2[i][counts2[i]-1];
    }

    return ANSWER(part1, part2);
}