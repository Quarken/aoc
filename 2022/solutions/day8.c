#include "aoc.h"

typedef struct { int x, y; } vec2;

static void test_visibility(int* heightmap, int* vismap, int pitch, vec2 start, vec2 direction, vec2 direction_y, int moves_x, int moves_y)
{
    for (int j = 0; j < moves_y; j++)
    {
        vec2 position = {
            .x = start.x + direction_y.x * j,
            .y = start.y + direction_y.y * j
        };
        vec2 initial = { position.x - direction.x, position.y - direction.y };
        int tallest_so_far = heightmap[initial.y * pitch + initial.x];
        for (int i = 0; i < moves_x; i++)
        {
            int h = heightmap[position.y * pitch + position.x];
            if (h > tallest_so_far)
            {
                vismap[position.y * pitch + position.x] = 1;
                tallest_so_far = h;
            }
            if (h == 9)
            {
                break;
            }
            position.x += direction.x;
            position.y += direction.y;
        }
    }
}

static int compute_scenic_score(int* heightmap, int width, int height, vec2 start, vec2 direction)
{
    int score = 0;
    int h = heightmap[start.y * width + start.x];
    vec2 pos = {
        .x = start.x + direction.x,
        .y = start.y + direction.y
    };
    while (pos.x >= 0 && pos.x < width && pos.y >= 0 && pos.y < height)
    {
        score++;
        if (heightmap[pos.y * width + pos.x] >= h)
        {
            break;
        }
        pos.x += direction.x;
        pos.y += direction.y;
    }
    return score == 0 ? 1 : score;
}

AOC_SOLUTION(8)(char* input, int input_length)
{
    int w = line_length(input);
    int h = (input_length + 1) / (w + 1);
    
    int* heightmap = alloc_array(w * h, int);
    int* vismap = alloc_array(w * h, int);
    for (int y = 0; y < h; y++)
    {
        for (int x = 0; x < w; x++)
        {
            // width+1 to account for newlines
            char c = input[y * (w+1) + x];
            heightmap[y * w + x] = c - '0';
        }
    }

    test_visibility(heightmap, vismap, w, (vec2){1,   1}, (vec2){ 1,  0}, (vec2){0,1}, w-2, h-2);
    test_visibility(heightmap, vismap, w, (vec2){w-2, 1}, (vec2){-1,  0}, (vec2){0,1}, w-2, h-2);

    // transpose
    test_visibility(heightmap, vismap, w, (vec2){1,   1}, (vec2){ 0,  1}, (vec2){1,0}, h-2, w-2);
    test_visibility(heightmap, vismap, w, (vec2){1, h-2}, (vec2){ 0, -1}, (vec2){1,0}, h-2, w-2);

    int interior_visible = 0;
    for (int y = 0; y < h; y++)
    {
        for (int x = 0; x < w; x++)
        {
            interior_visible += vismap[y * w + x];
        }
    }
    int part1 = 2 * w + 2 * h - 4 + interior_visible;

    int part2 = 0;
    for (int y = 1; y < h-1; y++)
    {
        for (int x = 1; x < w-1; x++)
        {
            vec2 start = {x, y};
            int scenic_score = 1;
            scenic_score *= compute_scenic_score(heightmap, w, h, start, (vec2){ 1,  0});
            scenic_score *= compute_scenic_score(heightmap, w, h, start, (vec2){-1,  0});
            scenic_score *= compute_scenic_score(heightmap, w, h, start, (vec2){ 0, -1});
            scenic_score *= compute_scenic_score(heightmap, w, h, start, (vec2){ 0,  1});
            part2 = max(part2, scenic_score);
        }
    }

    return ANSWER(part1, part2);
}