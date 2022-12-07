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

static int calculate_part1(fs_node* root)
{
    if (root == NULL) return 0;
    if (root->size <= 100000) return root->size + calculate_part1(root->next) + calculate_part1(root->child);
    return calculate_part1(root->next) + calculate_part1(root->child);
}

static void calculate_part2(fs_node* root, int required_space, int* min_so_far)
{
    if (root == NULL) return;
    if (root->size > required_space && root->size < (*min_so_far))
    {
        *min_so_far = root->size;
    }

    calculate_part2(root->next, required_space, min_so_far);
    calculate_part2(root->child, required_space, min_so_far);
}

AOC_SOLUTION(7)(char* input, int input_length)
{
    string input_str = { .data = input, .length = input_length };
    string_split_result lines = split_by(input_str, '\n');

    fs_node root = {
        .size = 0,
        .name = ""
    };
    fs_node* tree_root = &root;
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

    int part1 = calculate_part1(tree_root);
    int unused_space = 70000000 - root.size;
    int required_space = 30000000 - unused_space;
    int part2 = INT32_MAX;
    calculate_part2(tree_root, required_space, &part2);
    return ANSWER(part1, part2);
}