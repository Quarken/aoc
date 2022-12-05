#include "aoc.h"

#pragma warning(push, 0)
#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#pragma warning(pop)

static struct
{
    void* base;
    SIZE_T size;
    SIZE_T allocated;
    SIZE_T committed;
    SIZE_T commit_size;
} arena;

void platform_init_memory_arena()
{
    if (arena.base != NULL)
    {
        return;
    }

    SYSTEM_INFO sys_info;
    GetSystemInfo(&sys_info);

    // 8 gigabytes arena
    SIZE_T size = (SIZE_T)8 << 30;
    arena.base = VirtualAlloc(NULL, size, MEM_RESERVE, PAGE_NOACCESS);
    arena.size = size;
    arena.commit_size = sys_info.dwPageSize;

    // commit 16 megabytes to start with
    SIZE_T initial_commit = 16ULL << 20;
    VirtualAlloc(arena.base, initial_commit, MEM_COMMIT, PAGE_READWRITE);
    arena.committed += initial_commit;
}

void platform_reset_memory_arena()
{
    arena.allocated = 0;
}

void* alloc_size(size_t size)
{
    SIZE_T allocated = arena.allocated + size;
    if (allocated > arena.committed)
    {
        SIZE_T to_be_committed = size + arena.commit_size - 1;
        to_be_committed -= to_be_committed % arena.commit_size;
        VirtualAlloc((uint8_t*)arena.base + arena.committed, to_be_committed, MEM_COMMIT, PAGE_READWRITE);
        arena.committed += to_be_committed;
    }
    void* memory = (uint8_t*)arena.base + arena.allocated;
    arena.allocated += size;
    ZeroMemory(memory, size);
    return memory;
}

void* platform_load_function(const char* function_symbol)
{
    return (void*)GetProcAddress(GetModuleHandleA(NULL), function_symbol);
}

int64_t platform_run_timed_microseconds(aoc_solution_func function, char* input, int input_length)
{
    LARGE_INTEGER starting_time, ending_time, frequency;
    QueryPerformanceFrequency(&frequency);
    QueryPerformanceCounter(&starting_time);
    function(input, input_length);
    QueryPerformanceCounter(&ending_time);
    return ((ending_time.QuadPart - starting_time.QuadPart) * 1000000) / frequency.QuadPart;
}

void platform_get_executable_name(char* buffer, int buffer_size)
{
    TCHAR module[MAX_PATH] = {0};
    DWORD len = GetModuleFileNameA(NULL, module, MAX_PATH);

    DWORD start_index = 0;
    for (int i = len-1; i >= 0; i--)
    {
        if (module[i] == '\\') {
            start_index = i + 1;
            break;
        }
    }

    int copy_size = len - start_index;
    copy_size = copy_size < buffer_size ? copy_size : buffer_size;
    ZeroMemory(buffer, buffer_size);
    CopyMemory(buffer, module + start_index, copy_size);
}
