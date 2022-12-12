#include "aoc.h"

typedef struct { 
    int x;
    int y;
} vec2;

typedef struct
{
    vec2 pos;
    int distance;
} node;

typedef struct
{
    node* buffer;
    int buffer_size;
    int start;
    int end;
} queue;

typedef struct
{
    int part1;
    int part2;
} bfs_result;

static void put(queue* q, node v)
{
    q->buffer[q->end++] = v;
    q->end = q->end & (q->buffer_size - 1);
}

static node get(queue* q)
{
    node result = q->buffer[q->start++];
    q->start = q->start & (q->buffer_size - 1);
    return result;
}

static bfs_result find_path(vec2 S, vec2 E, char* map, bool* visited, int width, int height)
{
    int first_a = 0;
    queue q = {
        .buffer = alloc_array(512, node),
        .buffer_size = 512
    };

    put(&q, (node){S, 0});
    visited[S.y * width + S.x] = true;

    while (q.start != q.end)
    {
        node current_node = get(&q);
        vec2 current_pos = current_node.pos;

        vec2 neighbors[] = {
            {current_pos.x,     current_pos.y - 1},
            {current_pos.x,     current_pos.y + 1},
            {current_pos.x - 1, current_pos.y    },
            {current_pos.x + 1, current_pos.y    }
        };
        for (int i = 0; i < ARRAY_SIZE(neighbors); i++)
        {
            vec2 next = neighbors[i];

            if (next.x < 0 || next.x >= width || next.y < 0 || next.y >= height) continue;

            char move_start = map[current_pos.y * width + current_pos.x];
            char move_end = map[next.y * width + next.x];
            if ((move_end + 1) < move_start) continue;

            if (visited[next.y * width + next.x]) continue;

            if (next.x == E.x && next.y == E.y)
            {
                return (bfs_result){
                    .part1 = current_node.distance + 1,
                    .part2 = first_a
                };
            }

            if (first_a == 0 && move_end == 'a')
            {
                first_a = current_node.distance + 1;
            }

            visited[next.y * width + next.x] = true;
            put(&q, (node){
                .pos = next,
                .distance = current_node.distance + 1
            });
        }
    }

    return (bfs_result){0,0};
}

AOC_SOLUTION(12)(char* input, int input_length)
{
    int width = line_length(input);
    int height = (input_length + 1)  / (width + 1);

    char* map = alloc_array(width * height, char);
    for (int y = 0; y < height; y++)
    {
        memcpy(map + y * width, input + y * (width+1), width);
    }

    vec2 S, E;
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            char c = map[y * width + x];
            if (c == 'S')
            {
                S = (vec2){x, y};
            }
            else if (c == 'E')
            {
                E = (vec2){x, y};
            }
        }
    }

    map[S.y * width + S.x] = 'a';
    map[E.y * width + E.x] = 'z';

    bool* visited = alloc_array(width * height, bool);
    bfs_result result = find_path(E, S, map, visited, width, height);

    return ANSWER(result.part1, result.part2);
}