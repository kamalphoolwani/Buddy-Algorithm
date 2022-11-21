// #include "header.h"

// using namespace std;

// void print_msg(const char *buf)
// {
//     // Can't use printf here because printf internally calls `malloc`
//     // char buf[1024];
//     // snprintf(buf, 1024, "%s:%d malloc(%zu): Allocated %zu bytes at %p\n",
//     //          __FILE__, __LINE__, size, allocSize, ret);
//     write(STDOUT_FILENO, buf, strlen(buf) + 1);
// }

// MallocHeader *getBuddy(int index, MallocHeader *block)
// {
//     ul addr_diff = (ul)pow(2, ceil(log2(block->size)));
//     auto it = arena.freeList[index].begin();
//     MallocHeader *buddy = NULL;
//     while (it != arena.freeList[index].end())
//     {
//         if (*it == block + addr_diff || *it == block - addr_diff)
//         {
//             // it is buddy
//             // remove it
//             buddy = *it;
//             arena.freeList[index].erase(it);
//             break;
//         }
//         it++;
//     }
//     return buddy;
// }

// void mergeBlock(MallocHeader *block)
// {
//     // block->size will be <= Buddy_size
//     int index = ceil(log2(block->size));
//     index = index - 3;
//     if (index >= MAX_ORDER)
//     {
//         // push free block to freeList
//         arena.freeList[index].push_back(block);
//         return;
//     }

//     MallocHeader *buddy = getBuddy(index, block);
//     if (!buddy)
//     {
//         arena.freeList[index].push_back(block);
//         return;
//     }

//     if (block < buddy)
//     {
//         block->size *= 2;
//         mergeBlock(block);
//     }
//     else
//     {
//         buddy->size *= 2;
//         mergeBlock(buddy);
//     }
// }

// void splitBlock(int index)
// {
//     MallocHeader *blockA = (MallocHeader *)arena.freeList[index].front();
//     blockA->size /= 2;
//     blockA->is_free = 1;
//     arena.freeList[index].erase(arena.freeList[index].begin()); // pop_front()

//     MallocHeader *blockB = (MallocHeader *)((char *)blockA + blockA->size);
//     blockB->size = blockA->size;
//     blockB->is_free = 1;

//     arena.freeList[index - 1].push_back(blockA);
//     arena.freeList[index - 1].push_back(blockB);
// }
// //Provide next addr which is multiple of alignment
// void *next_addr(size_t alignment, void *addr)
// {
//     ul skip_header = (ul)addr + MALLOC_HEADER_SIZE;
//     ul divided_addr = skip_header & ~(ul)(alignment - 1);
//     return (void *)(divided_addr + alignment);
// }
#include "header.h"
#define nl NULL
using namespace std;

void print_msg(const char *buf)
{
    // Can't use printf here because printf internally calls `malloc`
    // char buf[1024];
    // snprintf(buf, 1024, "%s:%d malloc(%zu): Allocated %zu bytes at %p\n",
    // __FILE__, __LINE__, size, allocSize, ret);

    write(STDOUT_FILENO, buf, strlen(buf) + 1);
}

MallocHeader *getBuddy(int index, MallocHeader *block)
{
    ul addr_diff;
    addr_diff = (ul)pow(2, ceil(log2(block->size)));
    auto it = arena.freeList[index].begin();
    MallocHeader *buddy;
    buddy = nl;
    while (it != arena.freeList[index].end())
    {
        if (*it == block + addr_diff)
        {
            // it is buddy
            // remove it
            buddy = *it;
            arena.freeList[index].erase(it);
            break;
        }
        else if (*it == block - addr_diff)
        {
            // it is buddy
            // remove it
            buddy = *it;
            arena.freeList[index].erase(it);
            break;
        }
        it++;
    }
    return buddy;
}

void mergeBlock(MallocHeader *block)
{
    // block->size will be <= Buddy_size
    int index;
    index = ceil(log2(block->size));
    index = index - 3;// because 8 byte block is stored at freeList[0]
    MallocHeader *buddy;
    if (index >= MAX_ORDER)
    {
        // push free block to freeList
        arena.freeList[index].push_back(block);
        return;
    }

    buddy = getBuddy(index, block);
    if (!buddy)
    {
        arena.freeList[index].push_back(block);
        return;
    }
    int tmp;
    if (block < buddy)
    {
        tmp = 2;
        block->size = block->size * tmp;
        mergeBlock(block);
    }
    else
    {
        tmp = 2;
        buddy->size = buddy->size * 2;
        mergeBlock(buddy);
    }
}
// pop one freeList[index] free block and break into two then push both splitted block to freeList[index-1]
void splitBlock(int index)
{
    MallocHeader *blockA;
    MallocHeader *blockB;
    blockA = (MallocHeader *)arena.freeList[index].front();
    int two;
    two = 2;
    blockA->size = blockA->size / two;
    int one;
    one = 1;
    blockA->is_free = one;
    arena.freeList[index].erase(arena.freeList[index].begin()); // pop_front()
    blockB = (MallocHeader *)((char *)blockA + blockA->size);
    int newindex;
    blockB->size = blockA->size;
    newindex = index - 1;
    arena.freeList[newindex].push_back(blockA);
    blockB->is_free = 1;
    arena.freeList[newindex].push_back(blockB);
}
// Provide next addr which is multiple of alignment
void *next_addr(size_t alignment, void *addr)
{
    ul skip_header;
    skip_header = (ul)addr + MALLOC_HEADER_SIZE;
    ul divided_addr;
    divided_addr = skip_header & ~(ul)(alignment - 1);
    return (void *)(divided_addr + alignment);
}