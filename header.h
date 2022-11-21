#include <iostream>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <sys/mman.h>
#include <vector>
#include <cmath>
#include <stdlib.h>
#include <cstring>

#define MIN_BLOCK 8
#define MAX_ORDER 8
#define BUDDY_SIZE 2048
#define ul unsigned long

using namespace std;

struct MallocHeader
{
    size_t size;//header_size + actual_size(required by user)
    bool is_free;
    void *header_addr;
};

struct MemArena
{
    void *heap;
    vector<vector<struct MallocHeader *>>freeList;
    MemArena(){
        freeList.reserve(MAX_ORDER + 1);
    }
    /* order: 0, 1,  2,  ..., 8
       block: 8, 16, 32, ..., 2048 */
};

extern MemArena arena;
extern pthread_mutex_t malloc_lock;

void *my_malloc(size_t size);
void my_free(void *ptr);
void *my_calloc(size_t nmemb, size_t size);
void *my_realloc(void *ptr, size_t size);
void *my_reallocarray(void *ptr, size_t nmemb, size_t size);
void *my_memalign(size_t alignment, size_t size);
int my_posix_memalign(void **memptr, size_t alignment, size_t size);


//Utils functions

void print_msg(const char *buf);
void mergeBlock(MallocHeader *block);
MallocHeader * getBuddy(int index, MallocHeader * block);
void splitBlock(int index);
void *next_addr(size_t alignment, void *addr);

#define MALLOC_HEADER_SIZE (sizeof(MallocHeader))
#define MALLOC_BASE(addr) ((void *)((char *)addr - MALLOC_HEADER_SIZE))
