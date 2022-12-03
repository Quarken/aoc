#include "aoc.h"
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <float.h>

#ifndef AOC_YEAR
#define AOC_YEAR 2022
#endif

#define MAX_RUNS 10000

#define RESET "\x1B[0m"
#define YELLOW "\x1B[33m"

#define RED(str) "\x1B[31m" str RESET
#define BLU(str) "\x1B[34m" str RESET

void usage()
{
    char basename[255];
    platform_get_executable_name(basename, ARRAY_SIZE(basename));
    printf("Usage: %s solution [runs]\n\n", basename);
}

void print_solution(int i, aoc_part_answer ans)
{
    printf("  Part %i: " YELLOW, i);
    switch (ans.type)
    {
        case AOC_ANSWER_INT:
        {
            printf("%lli" RESET "\n", ans.int_answer);
            break;
        }

        case AOC_ANSWER_STRING:
        {
            // remove trailing/leading newlines and do a padded print
            char* string = ans.string_answer;
            const char* spacing = "          ";

            if (*string != 0)
            {
                // trim leading/trailing newlines, do a padded print out.
                while (*string == '\n')
                {
                    string++;
                }

                size_t len = strlen(string);
                while (string[len-1] == '\n')
                {
                    len--;
                }
                string[len] = 0;

                char* line = string;
                for (int i = 0; i <= len; i++)
                {
                    char c = string[i];
                    if (c == '\n' || c == 0)
                    {
                        string[i] = 0;
                        if (line != string) fputs(spacing, stdout);
                        printf("%s\n", line);
                        line = string + i + 1;
                    }
                }
            }
            printf(RESET);
            break;
        }
    }
}

int main(int arg_count, char** args)
{
    aoc_solution_func* solutions[26] = {0};
    int solution_count = ARRAY_SIZE(solutions) - 1;

    for (int i = 1; i < ARRAY_SIZE(solutions); i++)
    {
        char function_symbol[128];
        snprintf(function_symbol, sizeof(function_symbol), STRINGIFY(AOC_PREFIX) "%i", i);
        solutions[i] = (aoc_solution_func*)platform_load_function(function_symbol);
    }

    long solution_index = 0;
    int runs = 1;

    if (arg_count < 2)
    {
        usage();
        return -1;
    }

    solution_index = strtol(args[1], NULL, 10);
    if (solution_index < 1 || solution_index > solution_count)
    {
        printf("Error: specified solution is not between 1 and %i\n", solution_count);
        return -1;
    }

    if (solutions[solution_index] == NULL)
    {
        printf("Error: solution for day %li has not yet been implemented.", solution_index);
        return -1;
    }

    if (arg_count >= 3)
    {
        long runs_input = strtol(args[2], NULL, 10);
        if (runs_input < 1 || runs_input > MAX_RUNS)
        {
            printf("Error: specified runs is not between 1 and %i\n", MAX_RUNS);
            return -1;
        }
        runs = runs_input;
    }

    platform_init_memory_arena();

    // read input file
    char* input;
    int input_length = 0;
    {
        char input_file[32];
        snprintf(input_file, sizeof(input_file), "%i/inputs/day%li.txt", AOC_YEAR, solution_index);
        FILE *file = fopen(input_file, "rb");

        if (file == NULL)
        {
            printf("Error: could not find file %s\n", input_file);
            return -1;
        }

        fseek(file, 0, SEEK_END);
        long file_size = ftell(file);
        rewind(file);

        input = malloc(file_size + 1);
        size_t read = fread(input, 1, file_size, file);
        if (read != file_size)
        {
            printf("Error: failed reading %s into memory\n", input_file);
            return -1;
        }
        fclose(file);

        // replace \r\n with \n in place.
        int new_length = 0;
        for (int i = 0; i < file_size; i++)
        {
            if (input[i] != '\r')
            {
                input[new_length++] = input[i];
            }
        }
        input[new_length] = 0;
        input_length = new_length;
    }

    float min_time = FLT_MAX;
    float max_time = -FLT_MAX;
    double avg_time = 0.0;
    for (int i = 0; i < runs; i++)
    {
        platform_reset_memory_arena();
        int64_t time_us = platform_run_timed_microseconds(solutions[solution_index], input, input_length);
        float time = time_us / 1000.f;
        min_time = min_time < time ? min_time : time;
        max_time = max_time > time ? max_time : time;
        avg_time += ((double)time) / runs;
    }

    // NOTE: the solution may leave its answer as a string on the arena, fine as long as we dont reinit.
    platform_reset_memory_arena();
    aoc_answer ans = solutions[solution_index](input, input_length);

    printf(
        "Advent of Code %i day "RED("%li")"\n"
        "-------------------------------------------------------------\n"
        "Timings for "RED("%i")" runs (ms)\n"
        "  Avg: "BLU("%.3f")"\n"
        "  Min: "BLU("%.3f")"\n"
        "  Max: "BLU("%.3f")"\n"
        "-------------------------------------------------------------\n"
        "Solutions\n",
        AOC_YEAR, solution_index, runs, avg_time, min_time, max_time
    );

    print_solution(1, ans.part1);
    print_solution(2, ans.part2);
    printf("\n");
    return 0;
}