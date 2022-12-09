#include "aoc.h"

typedef union 
{ 
    struct { int32_t x, y; };
    int64_t key;
} vec2;

typedef struct 
{ 
    vec2* data; 
    int count;
} hash_set;

#define NUM_SLOTS 8192

static inline void insert(hash_set* set, vec2 v)
{
    if (v.key == 0) return;

    int index = (v.x * 83 + v.y * 131) & (NUM_SLOTS-1);
    while (set->data[index].key != 0 && set->data[index].key != v.key)
    {
        index = (index + 1) & (NUM_SLOTS-1);
    }

    if (set->data[index].key == 0)
    {
        set->data[index] = v;
        set->count++;
    }
}

AOC_SOLUTION(9)(char* input, int input_length)
{
    // +x is right, +y is up
    vec2 body[10] = {0};

    hash_set part1 = {
        .data = alloc_array(NUM_SLOTS, vec2),
        .count = 1
    };
    hash_set part2 = {
        .data = alloc_array(NUM_SLOTS, vec2),
        .count = 1
    };

    int index = 0;
    while (index < input_length)
    {
        string line = {input + index, line_length(input + index)};
        index += line.length + 1;

        char move_type = line.data[0];
        int move_count = parse_int((string){ .data = line.data + 2, .length = line.length - 2 });
        for (int move = 0; move < move_count; move++)
        {
            switch (move_type)
            {
                case 'R': body[0].x += 1; break;
                case 'L': body[0].x -= 1; break;
                case 'U': body[0].y += 1; break;
                case 'D': body[0].y -= 1; break;
                default: ASSERT(false);
            }

            for (int body_index = 1; body_index < ARRAY_SIZE(body); body_index++)
            {
                vec2 a = body[body_index-1];
                vec2 b = body[body_index];
                vec2 delta = { .x = a.x - b.x, .y = a.y - b.y };
                int distance = max(absolute(delta.x), absolute(delta.y));
                if (distance > 1)
                {
                    body[body_index].x += sign(delta.x);
                    body[body_index].y += sign(delta.y);
                }
            }

            insert(&part1, body[1]);
            insert(&part2, body[9]);
        }
    }

    return ANSWER(part1.count, part2.count);
}
