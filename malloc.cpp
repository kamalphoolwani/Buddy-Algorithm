#include "header.h"
#define T true
#define nl NULL
using namespace std;

MemArena arena;
pthread_mutex_t malloc_lock;

// provide free block of size >= allocSize which is near 2^(something)
void *buddy_malloc(size_t allocSize)
{
    // allocsize = size + header
    int order;
    MallocHeader *hdr;
    // allocSize == 8 * 2^order
    order = ceil(log2(allocSize));
    int idx;
    idx = max(order - 3, 0); // because 8 byte block is stored at freeList[0]
    pthread_mutex_lock(&malloc_lock);
    // freelist[order].size 0 or not i.e. freeList[order] has free block or not
    if (arena.freeList[idx].size())
    {

        hdr = arena.freeList[idx].front();
        // pop_front(arena.freeList[idx])
        arena.freeList[idx].erase(arena.freeList[idx].begin());
        pthread_mutex_unlock(&malloc_lock);
        return hdr;
    }
    // freeList[idx] has no free block
    int it = idx;
    while (++it <= MAX_ORDER)
    {
        size_t arenafreelistsize;
        arenafreelistsize = arena.freeList[it].size();
        if (arenafreelistsize)
        {
            while (it > idx)
            {
                splitBlock(it);
                it--;
            }
            // freeList[idx] has two free blocks
            hdr = arena.freeList[idx].front();
            // pop_front(arena.freeList[idx])
            arena.freeList[idx].erase(arena.freeList[idx].begin());
            pthread_mutex_unlock(&malloc_lock);
            return hdr;
        }
    }
    // All freeList[i].size() is 0
    hdr = (MallocHeader *)sbrk(BUDDY_SIZE);//sbrk(incr) function is used to change the space allocated for the calling process. The change is made by adding incr bytes to the process's break value and allocating the appropriate amount of space.
    if (hdr == (void *)-1)
    {
        return hdr;
    }
    // BUDDY_SIZE block is created and annotate as free
    hdr->size = BUDDY_SIZE;
    hdr->is_free = T;
    arena.freeList[MAX_ORDER].push_back(hdr);
    pthread_mutex_unlock(&malloc_lock);

    return buddy_malloc(allocSize);
}

void *my_malloc(size_t size)
{

    void *ret;
    size_t allocSize;
    MallocHeader *hdr;
    if (size <= 0)
    {
        return nl;
    }
    allocSize = size + MALLOC_HEADER_SIZE;

    if (allocSize <= BUDDY_SIZE)
    {

        void *nevvalue;
        ret = buddy_malloc(allocSize);
        nevvalue = (void *)-1;

        assert(ret != nevvalue);
        string str = "ok";
        hdr = (MallocHeader *)ret;
        hdr->size = allocSize;

        if (arena.heap == nl)
            arena.heap = ret;
        // print_msg(str.c_str());

        return (char *)ret + MALLOC_HEADER_SIZE;
    }
    else if (allocSize > BUDDY_SIZE)
    {
        void *zerovalue;
        zerovalue = 0;
        int negvalue;
        negvalue = -1;
        ret = mmap(zerovalue, allocSize, PROT_READ | PROT_WRITE,
                   MAP_ANONYMOUS | MAP_PRIVATE, negvalue, 0);
        void *nevvalue;
        nevvalue = (void *)-1;
        assert(ret != nevvalue);
        string str = "ok";
        hdr = (MallocHeader *)ret;
        hdr->size = allocSize;

        if (arena.heap == nl)
            arena.heap = ret;
        

        return (char *)ret + MALLOC_HEADER_SIZE;
    }
    return (char *)ret + MALLOC_HEADER_SIZE;
}

//This directive is used to run function when a shared library is loaded, typically during program startup. Before main()
__attribute__((constructor)) void init()
{
    arena.heap = nl;
    pthread_mutex_init(&malloc_lock, nl);
}
