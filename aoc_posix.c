#include "aoc.h"

#include <dlfcn.h>
#include <sys/mman.h>
#include <stddef.h>
#include <unistd.h>
#include <string.h>
#include <time.h>

static struct
{
    void* base;
    size_t size;
    size_t allocated;
    size_t committed;
    size_t commit_size;
} arena;

void platform_init_memory_arena()
{
    if (arena.base != NULL)
    {
        return;
    }

    // 8 gigabytes arena
    size_t size = (size_t)8 << 30;
    arena.base = mmap(NULL, size, PROT_NONE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    arena.size = size;
    arena.commit_size = sysconf(_SC_PAGE_SIZE);

    // commit 16 megabytes to start with
    size_t initial_commit = 16ULL << 20;
    mprotect(arena.base, initial_commit, PROT_READ | PROT_WRITE);
    arena.committed += initial_commit;
}

void platform_reset_memory_arena()
{
    arena.allocated = 0;
}

void* amalloc(size_t size)
{
    size_t allocated = arena.allocated + size;
    if (allocated > arena.committed)
    {
        size_t to_be_committed = size + arena.commit_size - 1;
        to_be_committed -= to_be_committed % arena.commit_size;
        mprotect((uint8_t*)arena.base + arena.committed, to_be_committed, PROT_READ | PROT_WRITE);
        arena.committed += to_be_committed;
    }
    void* memory = (uint8_t*)arena.base + arena.allocated;
    arena.allocated += size;
    memset(memory, 0, size);
    return memory;
}

void* platform_load_function(const char* function_symbol)
{
    return (void*)dlsym(dlopen(NULL, RTLD_LAZY), function_symbol);
}

int64_t platform_run_timed_microseconds(aoc_solution_func function, char* input)
{
    struct timespec start_ts, end_ts;
    clock_gettime(CLOCK_MONOTONIC_RAW, &start_ts);
    function(input);
    clock_gettime(CLOCK_MONOTONIC_RAW, &end_ts);
    int64_t start = start_ts.tv_sec * 1000000ULL + start_ts.tv_nsec / 1000;
    int64_t end = end_ts.tv_sec * 1000000ULL + start_ts.tv_nsec / 1000;
    return end - start;
}

void platform_get_executable_name(char* buffer, int buffer_size)
{
    char path[255] = {0};

    int ret = readlink("/proc/self/exe", path, sizeof(path)-1);
    if (ret == -1)
    {
        memcpy(buffer, "aoc", sizeof("aoc"));
        return;
    }


    int start_index = 0;
    for (int i = 0; i < ARRAY_SIZE(path); i++)
    {
        if (path[i] == 0) break;
        if (path[i] == '/')
        {
            start_index = i + 1;
        }
    }

    int copy_size = ARRAY_SIZE(path);
    copy_size = copy_size < buffer_size ? copy_size : buffer_size;
    memset(buffer, 0, buffer_size);
    memcpy(buffer, path, copy_size);
}
