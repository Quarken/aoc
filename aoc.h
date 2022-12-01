#pragma once
#include <stdint.h>
#include <string.h>

#if defined(_MSC_VER)
#define DLLEXPORT __declspec(dllexport)
#else
#define DLLEXPORT
#endif

#define ARRAY_SIZE(array) (sizeof(array)/sizeof((array)[0]))
#define UNUSED(x) (void)(x);
#define STRINGIFY_INTERNAL(s) #s
#define STRINGIFY(s) STRINGIFY_INTERNAL(s)
#define CONCAT_INTERNAL(a, b) a##b
#define CONCAT(a, b) CONCAT_INTERNAL(a, b)

#define AOC_PREFIX _aoc_solution_day
#define AOC_SOLUTION(i) DLLEXPORT aoc_answer CONCAT(AOC_PREFIX, i)

typedef enum
{
    AOC_ANSWER_INT,
    AOC_ANSWER_STRING
} aoc_answer_type;

typedef struct
{
    aoc_answer_type type;
    union
    {
        long long int_answer;
        char* string_answer;
    };
} aoc_part_answer;

typedef struct 
{
    aoc_part_answer part1;
    aoc_part_answer part2;
} aoc_answer;

static inline aoc_part_answer string_answer(char* answer)
{
    return (aoc_part_answer){ .type = AOC_ANSWER_STRING, .string_answer = answer };
}

static inline aoc_part_answer int_answer(long long answer)
{
    return (aoc_part_answer){ .type = AOC_ANSWER_INT, .int_answer = answer };
}

#define PART_ANSWER(ans) _Generic((ans),    \
    char*:          string_answer,          \
    int:            int_answer,             \
    long long:      int_answer              \
)(ans)

#define ANSWER(p1, p2) (aoc_answer){ .part1 = PART_ANSWER(p1), .part2 = PART_ANSWER(p2) }

typedef aoc_answer aoc_solution_func(char* input);

void platform_init_memory_arena();
void platform_reset_memory_arena();
void* platform_load_function(const char* function_symbol);
int64_t platform_run_timed_microseconds(aoc_solution_func function, char* input);
void platform_get_executable_name(char* buffer, int buffer_size);
void* amalloc(size_t size);
