#include "aoc.h"

typedef struct fs_node
{
    int32_t size;
    char name[32];
    struct fs_node* parent;
    struct fs_node* next;
    struct fs_node* child;
} fs_node;

static fs_node* find_in_directory(fs_node* dir, const char* name, int name_size)
{
    fs_node* current = dir->child;
    while (current != NULL && strncmp(current->name, name, name_size) != 0)
    {
        current = current->next;
    }
    return current;
}

static void recurse_and_calculate(fs_node* root, int required_space, int* part1, int* part2)
{
    if (root == NULL) return;
    if (root->size <= 100000) *part1 += root->size;
    if (root->size > required_space && root->size < (*part2))
    {
        *part2 = root->size;
    }

    recurse_and_calculate(root->next, required_space, part1, part2);
    recurse_and_calculate(root->child, required_space, part1, part2);
}

AOC_SOLUTION(7)(char* input, int input_length)
{
    string input_str = { .data = input, .length = input_length };
    string_split_result lines = split_by(input_str, '\n');

    fs_node filesystem_root = {
        .size = 0,
        .name = ""
    };

    fs_node* tree_root = &filesystem_root;
    fs_node* current = tree_root;
    for (int line_index = 1; line_index < lines.count; line_index++)
    {
        string line = lines.strings[line_index];
        if (line.data[0] == '$')
        {
            // command
            if (line.data[2] == 'c')
            {
                if (line.data[5] == '.')
                {
                    current = current->parent;
                }
                else
                {
                    current = find_in_directory(current, line.data + 5, line.length - 5);
                    ASSERT(current != NULL);
                }
            }
            // if it wasn't cd then it was ls, we dont have to check.
        }
        else if (line.data[0] == 'd')
        {
            // directory
            fs_node* dir = find_in_directory(current, line.data + 4, line.length - 4);
            if (dir == NULL)
            {
                dir = alloc_struct(fs_node);
                memcpy(dir->name, line.data + 4, line.length - 4);
                dir->parent = current;
                dir->next = current->child;
                current->child = dir;
            }
        }
        else
        {
            // file
            int size = parse_int(line);
            fs_node* search = current;
            while (search != NULL)
            {
                search->size += size;
                search = search->parent;
            }
        }
    }

    int unused_space = 70000000 - filesystem_root.size;
    int required_space = 30000000 - unused_space;
    int part1 = 0;
    int part2 = INT32_MAX;
    recurse_and_calculate(tree_root, required_space, &part1, &part2);
    return ANSWER(part1, part2);
}